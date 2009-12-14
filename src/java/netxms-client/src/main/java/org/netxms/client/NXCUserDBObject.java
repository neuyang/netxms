package org.netxms.client;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.UUID;
import java.util.Map.Entry;

import org.netxms.base.NXCPCodes;
import org.netxms.base.NXCPMessage;

public abstract class NXCUserDBObject
{
	// System-wide rights
	public static final int SYSTEM_ACCESS_MANAGE_USERS = 0x00000001;
	public static final int SYSTEM_ACCESS_SERVER_CONFIG = 0x00000002;
	public static final int SYSTEM_ACCESS_CONFIGURE_TRAPS = 0x00000004;
	public static final int SYSTEM_ACCESS_MANAGE_SESSIONS = 0x00000008;
	public static final int SYSTEM_ACCESS_VIEW_EVENT_DB = 0x00000010;
	public static final int SYSTEM_ACCESS_EDIT_EVENT_DB = 0x00000020;
	public static final int SYSTEM_ACCESS_EPP = 0x00000040;
	public static final int SYSTEM_ACCESS_MANAGE_ACTIONS = 0x00000080;
	public static final int SYSTEM_ACCESS_DELETE_ALARMS = 0x00000100;
	public static final int SYSTEM_ACCESS_MANAGE_PACKAGES = 0x00000200;
	public static final int SYSTEM_ACCESS_MANAGE_TOOLS = 0x00000800;
	public static final int SYSTEM_ACCESS_MANAGE_SCRIPTS = 0x00001000;
	public static final int SYSTEM_ACCESS_VIEW_TRAP_LOG = 0x00002000;
	public static final int SYSTEM_ACCESS_MANAGE_AGENT_CFG = 0x00008000;
	public static final int SYSTEM_ACCESS_MANAGE_SITUATIONS = 0x00010000;
	public static final int SYSTEM_ACCESS_SEND_SMS = 0x00020000;
	public static final int SYSTEM_ACCESS_MANAGE_MAPS = 0x00040000;
	public static final int SYSTEM_ACCESS_REGISTER_AGENTS = 0x00080000;
	public static final int SYSTEM_ACCESS_READ_FILES = 0x00100000;
	
	public static final int OBJECT_ACCESS_READ          = 0x00000001;
	public static final int OBJECT_ACCESS_MODIFY        = 0x00000002;
	public static final int OBJECT_ACCESS_CREATE        = 0x00000004;
	public static final int OBJECT_ACCESS_DELETE        = 0x00000008;
	public static final int OBJECT_ACCESS_READ_ALARMS   = 0x00000010;
	public static final int OBJECT_ACCESS_ACL           = 0x00000020;
	public static final int OBJECT_ACCESS_ACK_ALARMS    = 0x00000040;
	public static final int OBJECT_ACCESS_SEND_EVENTS   = 0x00000080;
	public static final int OBJECT_ACCESS_CONTROL       = 0x00000100;
	public static final int OBJECT_ACCESS_TERM_ALARMS   = 0x00000200;
	public static final int OBJECT_ACCESS_PUSH_DATA     = 0x00000400;
	
	// Object flags
	public static final int MODIFIED = 0x0001;
	public static final int DELETED = 0x0002;
	public static final int DISABLED = 0x0004;
	public static final int CHANGE_PASSWORD = 0x0008;
	public static final int CANNOT_CHANGE_PASSWORD = 0x0010;
	public static final int INTRUDER_LOCKOUT = 0x0020;
	
	protected long id;
	protected String name;
	protected UUID guid;
	protected int systemRights;
	protected int flags;
	protected String description;
	protected Map<String, String> customAttributes = new HashMap<String, String>(0);

	/**
	 * Default constructor
	 */
	public NXCUserDBObject(final String name)
	{
		this.name = name;
		description = "";
		guid = UUID.randomUUID();
	}
	
	/**
	 * Copy constructor
	 */
	public NXCUserDBObject(final NXCUserDBObject src)
	{
		this.id = src.id;
		this.name = new String(src.name);
		this.guid = UUID.fromString(src.guid.toString());
		this.systemRights = src.systemRights;
		this.flags = src.flags;
		this.description = src.description;
		this.customAttributes = new HashMap<String, String>(0);
		Iterator<Entry<String, String>> it  = src.customAttributes.entrySet().iterator();
		while(it.hasNext())
		{
			Entry<String, String> e = it.next();
			this.customAttributes.put(new String(e.getKey()), new String(e.getValue()));
		}
	}

	/**
	 * Create object from NXCP message
	 * @param msg Message containing object's data
	 */
	protected NXCUserDBObject(final NXCPMessage msg)
	{
		id = msg.getVariableAsInt64(NXCPCodes.VID_USER_ID);
		name = msg.getVariableAsString(NXCPCodes.VID_USER_NAME);
		flags = msg.getVariableAsInteger(NXCPCodes.VID_USER_FLAGS);
		systemRights = msg.getVariableAsInteger(NXCPCodes.VID_USER_SYS_RIGHTS);
		description = msg.getVariableAsString(NXCPCodes.VID_USER_DESCRIPTION);
		guid = msg.getVariableAsUUID(NXCPCodes.VID_GUID);
		
		int count = msg.getVariableAsInteger(NXCPCodes.VID_NUM_CUSTOM_ATTRIBUTES);
		long varId = NXCPCodes.VID_CUSTOM_ATTRIBUTES_BASE;
		for(int i = 0; i < count; i++)
		{
			String name = msg.getVariableAsString(varId++);
			String value = msg.getVariableAsString(varId++);
			customAttributes.put(name, value);
		}
	}
	
	/**
	 * Fill NXCP message with object data
	 */
	public void fillMessage(final NXCPMessage msg)
	{
		msg.setVariableInt32(NXCPCodes.VID_USER_ID, (int)id);
		msg.setVariable(NXCPCodes.VID_USER_NAME, name);
		msg.setVariableInt16(NXCPCodes.VID_USER_FLAGS, flags);
		msg.setVariableInt32(NXCPCodes.VID_USER_SYS_RIGHTS, systemRights);
		msg.setVariable(NXCPCodes.VID_USER_DESCRIPTION, description);
		
		msg.setVariableInt32(NXCPCodes.VID_NUM_CUSTOM_ATTRIBUTES, customAttributes.size());
		long varId = NXCPCodes.VID_CUSTOM_ATTRIBUTES_BASE;
		Iterator<Entry<String, String>> it = customAttributes.entrySet().iterator();
		while(it.hasNext())
		{
			Entry<String, String> e = it.next();
			msg.setVariable(varId++, e.getKey());
			msg.setVariable(varId++, e.getValue());
		}
	}

	/**
	 * @return true if user is marked as deleted
	 */
	public boolean isDeleted()
	{
		return (flags & DELETED) == DELETED;
	}

	/**
	 * @return the id
	 */
	public long getId()
	{
		return id;
	}

	/**
	 * @param id the id to set
	 */
	public void setId(long id)
	{
		this.id = id;
	}

	/**
	 * @return the name
	 */
	public String getName()
	{
		return name;
	}

	/**
	 * @param name the name to set
	 */
	public void setName(String name)
	{
		this.name = name;
	}

	/**
	 * @return the guid
	 */
	public UUID getGuid()
	{
		return guid;
	}

	/**
	 * @return the systemRights
	 */
	public int getSystemRights()
	{
		return systemRights;
	}

	/**
	 * @param systemRights the systemRights to set
	 */
	public void setSystemRights(int systemRights)
	{
		this.systemRights = systemRights;
	}

	/**
	 * @return the flags
	 */
	public int getFlags()
	{
		return flags;
	}

	/**
	 * @param flags the flags to set
	 */
	public void setFlags(int flags)
	{
		this.flags = flags;
	}

	/**
	 * @return the description
	 */
	public String getDescription()
	{
		return description;
	}

	/**
	 * @param description the description to set
	 */
	public void setDescription(String description)
	{
		this.description = description;
	}

	/**
	 * Get custom attribute
	 * @param name Name of the attribute
	 */
	public String getCustomAttribute(final String name)
	{
		return customAttributes.get(name);
	}
	
	/**
	 * Set custom attribute's value
	 * @param name Name of the attribute
	 * @param value New value for attribute
	 */
	public void setCustomAttribute(final String name, final String value)
	{
		customAttributes.put(name, value);
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#clone()
	 */
	@Override
	public Object clone() throws CloneNotSupportedException
	{
		return super.clone();
	}
	
	/**
	 * Check if object is disabled
	 * @return true if object is disabled
	 */
	public boolean isDisabled()
	{
		return ((flags & DISABLED) == DISABLED) ? true : false;
	}
	
	/**
	 * Check if password should be changed at next logon
	 * @return true if password should be changed at next logon
	 */
	public boolean isPasswordChangeNeeded()
	{
		return ((flags & CHANGE_PASSWORD) == CHANGE_PASSWORD) ? true : false;
	}
	
	/**
	 * Check if password change is forbidden
	 * @return true if password change is forbidden
	 */
	public boolean isPasswordChangeForbidden()
	{
		return ((flags & CANNOT_CHANGE_PASSWORD) == CANNOT_CHANGE_PASSWORD) ? true : false;
	}
}
