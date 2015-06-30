/**
 * Java-Bridge NetXMS subagent
 * Copyright (C) 2013 TEMPEST a.s.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
package org.netxms.agent;

import java.io.File;
import java.lang.reflect.Constructor;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

/**
 * This class is a Java representation of native subagent exposing to it required APIs
 * It also exposes native sendTrap, pushData and writeMessage agent API
 */
public class SubAgent
{
   public enum LogLevel
   {
      INFO(0x0004), WARNING(0x0002), ERROR(0x0001);

      private int value;

      LogLevel(final int value)
      {
         this.value = value;
      }

      public int getValue()
      {
         return value;
      }
   }

   private static final String JAR = ".jar";
   private static final String PLUGIN_CLASSNAME_ATTRIBUTE_NAME = "NetXMS-Plugin-Classname";
   private static final String MANIFEST_PATH = "META-INF/MANIFEST.MF";

   protected Map<String, Plugin> plugins;
   protected Map<String, Action> actions;
   protected Map<String, Parameter> parameters;
   protected Map<String, ListParameter> lists;
   protected Map<String, PushParameter> pushParameters;
   protected Map<String, TableParameter> tables;

   private Config config = null;

   /**
    * Private constructor. Will be invoked by native wrapper only.
    * @param config
    */
   private SubAgent(Config config)
   {
      plugins = new HashMap<String, Plugin>();
      actions = new HashMap<String, Action>();
      parameters = new HashMap<String, Parameter>();
      lists = new HashMap<String, ListParameter>();
      pushParameters = new HashMap<String, PushParameter>();
      tables = new HashMap<String, TableParameter>();

      this.config = config;
      writeDebugLog(1, "Java SubAgent created");

      // load all Plugins
      ConfigEntry configEntry = config.getEntry("/Java/Plugin");
      if (configEntry != null)
      {
         for(int i = 0; i < configEntry.getValueCount(); i++)
         {
            String entry = configEntry.getValue(i).trim();
            try
            {
               loadPlugin(entry);
            }
            catch(Throwable e)
            {
               AgentWriteLog(LogLevel.ERROR, "JAVA: Exception in loadPlugin: " + e.getClass().getCanonicalName() + ": " + e.getMessage());
            }
         }
      }
      else
      {
         AgentWriteLog(LogLevel.WARNING, "No plugins defined in Java section");
      }
   }

   /**
    * @return
    */
   protected Config getConfig()
   {
      return config;
   }

   /*===== native methods exposed by agent (see nms_agent.h) =====*/
   
   /**
    * Extract argument from full parameter name.
    * 
    * For example:
    *   parameter: Process.Count(java, 2)
    *   
    *   AgentGetParameterArg(parameter, 1) will return "java"
    * 
    * @param param full parameter name (as passed to handler)
    * @param index argument index (first argument has index 1)
    * @return extracted argument on success, empty string if argument is missing, null if input is malformed
    */
   public static native String getParameterArg(String param, int index);

   public static native void sendTrap(int event, String name, String[] args);

   protected static native boolean pushParameterData(String name, String value);

   protected static native void writeLog(int level, String message);

   public static void AgentWriteLog(LogLevel level, String message)
   {
      writeLog(level.getValue(), message);
   }

   public static native void writeDebugLog(int level, String message);
   
   /*===== end of native methods exposed by agent =====*/

   /**
    * Initialize (to be called from native subagent)
    * 
    * @param config
    * @return
    */
   public boolean init(Config config)
   {
      for(Map.Entry<String, Plugin> entry : plugins.entrySet())
      {
         entry.getValue().init(config);
      }
      return true;
   }

   /**
    * Shutdown (to be called from native subagent)
    */
   public void shutdown()
   {
      writeDebugLog(2, "JAVA: subagent shutdown initiated");
      for(Map.Entry<String, Plugin> entry : plugins.entrySet())
      {
         try
         {
            writeDebugLog(5, "JAVA: calling shutdown() method for plugin " + entry.getKey());
            entry.getValue().shutdown();
         }
         catch(Throwable e)
         {
            writeDebugLog(2, "JAVA: exception in plugin " + entry.getKey() + " shutdown handler: " + e.getClass().getCanonicalName() + ": " + e.getMessage());
         }
      }
      writeDebugLog(2, "JAVA: subagent shutdown completed");
   }

   /**
    * will be called from subagent native initialization
    * 
    * @param path
    * @return
    */
   protected boolean loadPlugin(String path)
   {
      writeDebugLog(2, "SubAgent.loadPlugin(" + path + ")");
      Plugin plugin = null;
      try
      {
         plugin = path.toLowerCase().endsWith(JAR) ? createPluginWithJar(path, config) : createPluginWithClassname(path, config);
      }
      catch(Throwable e)
      {
         AgentWriteLog(LogLevel.WARNING, "Failed to load plugin " + path + ": " + e.getClass().getCanonicalName() + ": " + e.getMessage());
      }
      
      if (plugin == null)
         return false;
      
      plugins.put(plugin.getName(), plugin);
      // register actions
      Action[] _actions = plugin.getActions();
      for(int i = 0; i < _actions.length; i++)
         actions.put(createContributionItemId(plugin, _actions[i]), _actions[i]);
   
      // register paramaters
      Parameter[] _parameters = plugin.getParameters();
      for(int i = 0; i < _parameters.length; i++)
         parameters.put(createContributionItemId(plugin, _parameters[i]), _parameters[i]);
   
      // register list paramaters
      ListParameter[] _listParameters = plugin.getListParameters();
      for(int i = 0; i < _listParameters.length; i++)
         lists.put(createContributionItemId(plugin, _listParameters[i]), _listParameters[i]);
   
      // register push paramaters
      PushParameter[] _pushParameters = plugin.getPushParameters();
      for(int i = 0; i < _pushParameters.length; i++)
         pushParameters.put(createContributionItemId(plugin, _pushParameters[i]), _pushParameters[i]);
   
      // register table paramaters
      TableParameter[] _tableParameters = plugin.getTableParameters();
      for(int i = 0; i < _tableParameters.length; i++)
         tables.put(createContributionItemId(plugin, _tableParameters[i]), _tableParameters[i]);
   
      writeDebugLog(6, "SubAgent.loadPlugin actions=" + actions);
      writeDebugLog(6, "SubAgent.loadPlugin parameters=" + parameters);
      writeDebugLog(6, "SubAgent.loadPlugin listParameters=" + lists);
      writeDebugLog(6, "SubAgent.loadPlugin pushParameters=" + pushParameters);
      writeDebugLog(6, "SubAgent.loadPlugin tableParameters=" + tables);
      return true;
   }

   /**
    * @param classname
    * @param config
    * @return
    * @throws Exception
    */
   protected Plugin createPluginWithClassname(String classname, Config config) throws Exception
   {
      Class<?> pluginClass = Class.forName(classname);
      writeDebugLog(3, "SubAgent.createPluginWithClassname loaded class " + pluginClass);
      Plugin plugin = instantiatePlugin(pluginClass.asSubclass(Plugin.class), config);
      writeDebugLog(3, "SubAgent.createPluginWithClassname created instance " + plugin);
      return plugin;
   }

   /**
    * @param jarFile
    * @param config
    * @return
    * @throws Exception
    */
   protected Plugin createPluginWithJar(String jarFile, Config config) throws Exception
   {
      URLClassLoader classLoader = new URLClassLoader(new URL[] { new File(jarFile).toURI().toURL() }, Thread.currentThread().getContextClassLoader() );
      URL url = classLoader.findResource(MANIFEST_PATH);
      Manifest manifest = new Manifest(url.openStream());
      Attributes attributes = manifest.getMainAttributes();
      String pluginClassName = attributes.getValue(PLUGIN_CLASSNAME_ATTRIBUTE_NAME);
      if (pluginClassName == null)
      {
         classLoader.close();
         throw new Exception("Failed to find " + PLUGIN_CLASSNAME_ATTRIBUTE_NAME + " attribute in manifest of " + jarFile);
      }
      Class<?> pluginClass = Class.forName(pluginClassName, false, classLoader);
      Plugin plugin = instantiatePlugin(pluginClass.asSubclass(Plugin.class), config);
      return plugin;
   }

   /**
    * uses reflection to create an instance of Plugin
    * 
    * @param pluginClass
    * @param config
    * @return
    * @throws Exception
    */
   protected Plugin instantiatePlugin(Class<? extends Plugin> pluginClass, Config config) throws Exception
   {
      Plugin plugin = null;
      Constructor<? extends Plugin> constructor = pluginClass.getDeclaredConstructor(org.netxms.agent.Config.class);
      if (constructor != null)
      {
         plugin = constructor.newInstance(config);
      }
      else
      {
         throw new Exception("Failed to find constructor Plugin(Config) in class " + pluginClass);
      }
      return plugin;
   }

   /**
    * to be called from native subagent
    * 
    * @param param
    * @param id
    * @return
    */
   public String parameterHandler(final String param, final String id)
   {
      try
      {
         Parameter parameter = parameters.get(id);
         if (parameter != null)
         {
            return parameter.getValue(param);
         }
         return null;
      }
      catch(Throwable e)
      {
         writeDebugLog(6, "JAVA: Exception in parameter handler: " + e.getClass().getCanonicalName() + ": " + e.getMessage());
         return null;
      }
   }

   /**
    * to be called from native subagent
    * 
    * @param param
    * @param id
    * @return
    */
   public String[] listHandler(final String param, final String id)
   {
      try
      {
         ListParameter listParameter = lists.get(id);
         if (listParameter != null)
         {
            return listParameter.getValue(param);
         }
         return null;
      }
      catch(Throwable e)
      {
         writeDebugLog(6, "JAVA: Exception in list handler: " + e.getClass().getCanonicalName() + ": " + e.getMessage());
         return null;
      }
   }

   /**
    * to be called from native subagent
    * 
    * @param param
    * @param id
    * @return
    */
   public String[][] tableHandler(final String param, final String id)
   {
      try
      {
         TableParameter tableParameter = tables.get(id);
         if (tableParameter != null)
         {
            writeDebugLog(7, "SubAgent.tableParameterHandler(param=" + param + ", id=" + id + ") returning " + tableParameter.getValue(param));
            return tableParameter.getValue(param);
         }
         return null;
      }
      catch(Throwable e)
      {
         writeDebugLog(6, "JAVA: Exception in table handler: " + e.getClass().getCanonicalName() + ": " + e.getMessage());
         return null;
      }
   }

   /**
    * to be called from native subagent
    * 
    * @param param
    * @param args
    * @param id
    */
   public boolean actionHandler(final String param, final String[] args, final String id)
   {
      try
      {
         Action action = actions.get(id);
         if (action != null)
         {
            return action.execute(param, args);
         }
      }
      catch(Throwable e)
      {
         writeDebugLog(6, "JAVA: Exception in action handler: " + e.getClass().getCanonicalName() + ": " + e.getMessage());
      }
      return false;
   }

   /**
    * @return
    */
   public String[] getActions()
   {
      List<String> list = new ArrayList<String>();
      for(Entry<String, Action> e : actions.entrySet())
      {
         list.add(e.getKey());
         list.add(e.getValue().getName());
         list.add(e.getValue().getDescription());
      }
      return list.toArray(new String[0]);
   }

   /**
    * @return
    */
   public String[] getParameters()
   {
      List<String> list = new ArrayList<String>();
      for(Entry<String, Parameter> e : parameters.entrySet())
      {
         list.add(e.getKey());
         list.add(e.getValue().getName());
         list.add(e.getValue().getDescription());
         list.add(Integer.toString(e.getValue().getType().getValue()));
      }
      return list.toArray(new String[0]);
   }

   /**
    * @return
    */
   public String[] getLists()
   {
      List<String> list = new ArrayList<String>();
      for(Entry<String, ListParameter> e : lists.entrySet())
      {
         list.add(e.getKey());
         list.add(e.getValue().getName());
         list.add(e.getValue().getDescription());
      }
      return list.toArray(new String[0]);
   }

   /**
    * @return
    */
   public String[] getPushParameters()
   {
      List<String> list = new ArrayList<String>();
      for(Entry<String, PushParameter> e : pushParameters.entrySet())
      {
         list.add(e.getKey());
         list.add(e.getValue().getName());
         list.add(e.getValue().getDescription());
         list.add(Integer.toString(e.getValue().getType().getValue()));
      }
      return list.toArray(new String[0]);
   }

   /**
    * @return
    */
   public String[] getTables()
   {
      List<String> list = new ArrayList<String>();
      if (tables.size() > 0)
      {
         list.add(Integer.toString(tables.size()));
         for(Entry<String, TableParameter> e : tables.entrySet())
         {
            list.add(e.getKey());
            list.add(e.getValue().getName());
            list.add(e.getValue().getDescription());
            
            TableColumn[] columns = e.getValue().getColumns();
            list.add(Integer.toString(columns.length));
            for(TableColumn tc : columns)
            {
               list.add(tc.getName());
               list.add(tc.getDisplayName());
               list.add(Integer.toString(tc.getType().getValue()));
               list.add(tc.isInstance() ? "T" : "F");
            }
         }
      }
      return list.toArray(new String[0]);
   }
   
   /**
    * @param plugin
    * @param parameter
    * @return
    */
   private static String createContributionItemId(Plugin plugin, AgentContributionItem ci)
   {
      return plugin.getName() + "/" + ci.getName();
   }
}
