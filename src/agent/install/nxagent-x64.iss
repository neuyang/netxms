; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
#include "setup.iss"
OutputBaseFilename=nxagent-2.1.6-x64
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64

[Files]
; Installer helpers
Source: "..\..\..\x64\release\nxreload.exe"; DestDir: "{tmp}"; Flags: dontcopy signonce
; Agent
Source: "..\..\..\x64\release\libnetxms.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\libnxagent.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\libnxdb.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\libnxjava.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\libnxlp.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\libnxsnmp.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\appagent.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\nxagentd.exe"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\nxsagent.exe"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\db2.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\dbquery.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\devemu.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\ecs.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\filemgr.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\informix.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\java.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\logwatch.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\netsvc.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\odbcquery.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\oracle.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\ping.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\portcheck.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\sms.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\ssh.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\tuxedo.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\ups.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\winnt.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\winperf.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\wmi.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\db2.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\informix.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\mssql.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\mysql.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\oracle.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\pgsql.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\sqlite.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\contrib\nxagentd.conf-dist"; DestDir: "{app}\etc"; Flags: ignoreversion
Source: "..\..\..\x64\Release\libexpat.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\libpng.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\libtre.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\nxsqlite.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\nxzlib.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\Release\jansson.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\install\files\windows\x64\libeay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\install\files\windows\x64\ssleay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\install\files\windows\x64\libcurl.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\install\files\windows\x64\libssh.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\install\files\windows\x64\Microsoft.VC80.CRT\*"; DestDir: "{app}\bin\Microsoft.VC80.CRT"; Flags: ignoreversion
Source: "..\..\libnxjava\java\target\netxms-java-bridge.jar"; DestDir: "{app}\lib"; Flags: ignoreversion
Source: "..\subagents\bind9\target\bind9.jar"; DestDir: "{app}\lib"; Flags: ignoreversion
Source: "..\subagents\java\java\target\netxms-agent.jar"; DestDir: "{app}\lib"; Flags: ignoreversion
Source: "..\subagents\jmx\target\jmx.jar"; DestDir: "{app}\lib"; Flags: ignoreversion
Source: "..\subagents\ubntlw\target\ubntlw.jar"; DestDir: "{app}\lib"; Flags: ignoreversion
; Command-line tools
Source: "..\..\..\x64\release\nxappget.exe"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\nxapush.exe"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\nxevent.exe"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\nxpush.exe"; DestDir: "{app}\bin"; Flags: ignoreversion signonce
Source: "..\..\..\x64\release\libnxclient.dll"; DestDir: "{app}\bin"; Flags: ignoreversion signonce

;#include "custom.iss"

#include "common.iss"

Function GetCustomCmdLine(Param: String): String;
Begin
  Result := '';
End;
