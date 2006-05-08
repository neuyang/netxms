; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=NetXMS
AppVerName=NetXMS 0.2.11-rc1
AppVersion=0.2.11-rc1
AppPublisher=NetXMS Team
AppPublisherURL=http://www.netxms.org
AppSupportURL=http://www.netxms.org
AppUpdatesURL=http://www.netxms.org
DefaultDirName=C:\NetXMS
DefaultGroupName=NetXMS
AllowNoIcons=yes
LicenseFile=..\..\..\copying
OutputBaseFilename=netxms-0.2.11-rc1
Compression=lzma
SolidCompression=yes
LanguageDetectionMethod=none

[Components]
Name: "base"; Description: "Base Files"; Types: full compact custom; Flags: fixed
Name: "console"; Description: "Administrator's Console"; Types: full
Name: "server"; Description: "NetXMS Server"; Types: full compact
Name: "server\mssql"; Description: "Microsoft SQL DB-Library"; Types: full
Name: "server\mysql"; Description: "MySQL Client Library"; Types: full
Name: "server\pgsql"; Description: "PostgreSQL Client Library"; Types: full
Name: "websrv"; Description: "Web Server"; Types: full

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Common files
Source: "..\..\..\ChangeLog"; DestDir: "{app}\doc"; Flags: ignoreversion; Components: base
Source: "..\..\libnetxms\Release\libnetxms.dll"; DestDir: "{app}\bin"; BeforeInstall: StopAllServices; Flags: ignoreversion; Components: base
Source: "..\..\libnxcscp\Release\libnxcscp.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: base
; Executables and DLLs shared between different components (server, console, etc.)
Source: "..\..\libnxcl\Release\libnxcl.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console websrv
Source: "..\..\libnxmap\Release\libnxmap.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console server websrv
Source: "..\..\libnxsnmp\Release\libnxsnmp.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server console
Source: "..\..\libnxsl\Release\libnxsl.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server console
Source: "..\..\nxscript\Release\nxscript.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server console
Source: "..\..\server\tools\nxconfig\Release\nxconfig.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server websrv
; Server files
Source: "..\..\server\libnxsrv\Release\libnxsrv.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\core\Release\nxcore.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\netxmsd\Release\netxmsd.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\dbdrv\mysql\Release\mysql.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\dbdrv\mssql\Release\mssql.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\dbdrv\odbc\Release\odbc.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\dbdrv\pgsql\Release\pgsql.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\dbdrv\sqlite\Release\sqlite.ddr"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\smsdrv\generic\Release\generic.sms"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxaction\Release\nxaction.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxadm\Release\nxadm.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxdbmgr\Release\nxdbmgr.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxget\Release\nxget.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxsnmpget\Release\nxsnmpget.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxsnmpwalk\Release\nxsnmpwalk.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxsnmpset\Release\nxsnmpset.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\server\tools\nxupload\Release\nxupload.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\nxmibc\Release\nxmibc.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\core\Release\nxagentd.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\subagents\winnt\Release\winnt.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\subagents\win9x\Release\win9x.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\subagents\winperf\Release\winperf.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\subagents\ping\Release\ping.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\subagents\portCheck\Release\portcheck.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\agent\subagents\ups\Release\ups.nsm"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\..\..\sql\dbinit_mssql.sql"; DestDir: "{app}\lib\sql"; Flags: ignoreversion; Components: server
Source: "..\..\..\sql\dbinit_mysql.sql"; DestDir: "{app}\lib\sql"; Flags: ignoreversion; Components: server
Source: "..\..\..\sql\dbinit_oracle.sql"; DestDir: "{app}\lib\sql"; Flags: ignoreversion; Components: server
Source: "..\..\..\sql\dbinit_pgsql.sql"; DestDir: "{app}\lib\sql"; Flags: ignoreversion; Components: server
Source: "..\..\..\sql\dbinit_sqlite.sql"; DestDir: "{app}\lib\sql"; Flags: ignoreversion; Components: server
Source: "..\..\..\images\*.ico"; DestDir: "{app}\var\images"; Flags: ignoreversion; Components: server
Source: "..\..\..\images\*.png"; DestDir: "{app}\var\images"; Flags: ignoreversion; Components: server
Source: "..\..\..\contrib\mibs\*.txt"; DestDir: "{app}\var\mibs"; Flags: ignoreversion; Components: server
Source: "..\..\..\contrib\netxmsd.conf-dist"; DestDir: "{app}\etc"; Flags: ignoreversion; Components: server
Source: "..\..\..\contrib\nxagentd.conf-dist"; DestDir: "{app}\etc"; Flags: ignoreversion; Components: server
; Console files
Source: "..\..\console\nxuilib\Release\nxuilib.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console
Source: "..\..\console\nxlexer\Release\nxlexer.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console
Source: "..\..\console\win32\Release\nxcon.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console
Source: "..\..\console\nxav\Release\nxav.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console
; Web server files
Source: "..\..\nxhttpd\Release\nxhttpd.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: websrv
Source: "..\..\nxhttpd\static\*.xsl"; DestDir: "{app}\var\www"; Flags: ignoreversion; Components: websrv
Source: "..\..\nxhttpd\static\*.js"; DestDir: "{app}\var\www"; Flags: ignoreversion; Components: websrv
Source: "..\..\nxhttpd\static\screen.css"; DestDir: "{app}\var\www"; Flags: ignoreversion; Components: websrv
Source: "..\..\nxhttpd\static\images\*.png"; DestDir: "{app}\var\www\images"; Flags: ignoreversion; Components: websrv
Source: "..\..\nxhttpd\static\images\*.jpg"; DestDir: "{app}\var\www\images"; Flags: ignoreversion; Components: websrv
; Third party files
Source: "Files\mfc42.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console
Source: "Files\scilexer.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: console
Source: "Files\libmysql.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server\mysql
Source: "Files\ntwdblib.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server\mssql
Source: "Files\libpq.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server\pgsql
Source: "Files\libintl-2.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server\pgsql
Source: "Files\libiconv-2.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server\pgsql
Source: "Files\ssleay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: server\pgsql
Source: "Files\libeay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: base
Source: "Files\bgd.dll"; DestDir: "{app}\bin"; Flags: ignoreversion; Components: websrv

[Icons]
Name: "{group}\Alarm Viewer"; Filename: "{app}\bin\nxav.exe"; Components: console
Name: "{group}\NetXMS Console"; Filename: "{app}\bin\nxcon.exe"; Components: console
Name: "{group}\Server Configuration Wizard"; Filename: "{app}\bin\nxconfig.exe"; Components: server
Name: "{group}\Server Console"; Filename: "{app}\bin\nxadm.exe"; Parameters: "-i"; Components: server
Name: "{group}\{cm:UninstallProgram,NetXMS}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\NetXMS Console"; Filename: "{app}\bin\nxcon.exe"; Tasks: desktopicon; Components: console
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\NetXMS Console"; Filename: "{app}\bin\nxcon.exe"; Tasks: quicklaunchicon; Components: console

[Dirs]
Name: "{app}\etc"
Name: "{app}\database"
Name: "{app}\var\packages"

[Registry]
Root: HKLM; Subkey: "Software\NetXMS"; Flags: uninsdeletekeyifempty; Components: server websrv
Root: HKLM; Subkey: "Software\NetXMS\Server"; Flags: uninsdeletekey; Components: server websrv
Root: HKLM; Subkey: "Software\NetXMS\Server"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Components: server websrv
Root: HKLM; Subkey: "Software\NetXMS\Server"; ValueType: string; ValueName: "ConfigFile"; ValueData: "{app}\etc\netxmsd.conf"; Components: server

[Run]
Filename: "{app}\bin\nxmibc.exe"; Parameters: "-d ""{app}\var\mibs"" -o ""{app}\var\mibs\netxms.mib"""; WorkingDir: "{app}\bin"; StatusMsg: "Compiling MIB files..."; Flags: runhidden; Components: server
Filename: "{app}\bin\nxconfig.exe"; Parameters: "--create-agent-config"; WorkingDir: "{app}\bin"; StatusMsg: "Creating agent's configuration file..."; Components: server
Filename: "{app}\bin\nxagentd.exe"; Parameters: "-c ""{app}\etc\nxagentd.conf"" -I"; WorkingDir: "{app}\bin"; StatusMsg: "Installing agent service..."; Flags: runhidden; Components: server
Filename: "{app}\bin\nxagentd.exe"; Parameters: "-s"; WorkingDir: "{app}\bin"; StatusMsg: "Starting agent service..."; Flags: runhidden; Components: server
Filename: "{app}\bin\nxconfig.exe"; Parameters: "--configure-if-needed"; WorkingDir: "{app}\bin"; StatusMsg: "Running server configuration wizard..."; Components: server
;Filename: "{app}\bin\netxmsd.exe"; Parameters: "--config ""{app}\etc\netxmsd.conf"" install"; WorkingDir: "{app}\bin"; StatusMsg: "Installing core service..."; Flags: runhidden; Components: server
Filename: "{app}\bin\netxmsd.exe"; Parameters: "start"; WorkingDir: "{app}\bin"; StatusMsg: "Starting core service..."; Flags: runhidden; Components: server
Filename: "{app}\bin\nxconfig.exe"; Parameters: "--create-nxhttpd-config {code:GetMasterServer}"; WorkingDir: "{app}\bin"; StatusMsg: "Creating web server's configuration file..."; Components: websrv
Filename: "{app}\bin\nxhttpd.exe"; Parameters: "-c ""{app}\etc\nxhttpd.conf"" -I"; WorkingDir: "{app}\bin"; StatusMsg: "Installing web server service..."; Flags: runhidden; Components: websrv
Filename: "{app}\bin\nxhttpd.exe"; Parameters: "-s"; WorkingDir: "{app}\bin"; StatusMsg: "Starting web server service..."; Flags: runhidden; Components: websrv

[UninstallRun]
Filename: "{app}\bin\nxhttpd.exe"; Parameters: "-S"; StatusMsg: "Stopping web server service..."; RunOnceId: "StopWebService"; Flags: runhidden; Components: websrv
Filename: "{app}\bin\nxhttpd.exe"; Parameters: "-R"; StatusMsg: "Uninstalling web server service..."; RunOnceId: "DelWebService"; Flags: runhidden; Components: websrv
Filename: "{app}\bin\netxmsd.exe"; Parameters: "stop"; StatusMsg: "Stopping core service..."; RunOnceId: "StopCoreService"; Flags: runhidden; Components: server
Filename: "{app}\bin\netxmsd.exe"; Parameters: "remove"; StatusMsg: "Uninstalling core service..."; RunOnceId: "DelCoreService"; Flags: runhidden; Components: server
Filename: "{app}\bin\nxagentd.exe"; Parameters: "-S"; StatusMsg: "Stopping agent service..."; RunOnceId: "StopAgentService"; Flags: runhidden; Components: server
Filename: "{app}\bin\nxagentd.exe"; Parameters: "-R"; StatusMsg: "Uninstalling agent service..."; RunOnceId: "DelAgentService"; Flags: runhidden; Components: server

[Code]
Var
  HttpdSettingsPage: TInputQueryWizardPage;

Procedure StopAllServices;
Var
  strExecName : String;
  iResult : Integer;
Begin
  strExecName := ExpandConstant('{app}\bin\netxmsd.exe');
  If FileExists(strExecName) Then
  Begin
    Exec(strExecName, 'stop', ExpandConstant('{app}\bin'), 0, ewWaitUntilTerminated, iResult);
  End;

  strExecName := ExpandConstant('{app}\bin\nxhttpd.exe');
  If FileExists(strExecName) Then
  Begin
    Exec(strExecName, '-S', ExpandConstant('{app}\bin'), 0, ewWaitUntilTerminated, iResult);
  End;

  strExecName := ExpandConstant('{app}\bin\nxagentd.exe');
  If FileExists(strExecName) Then
  Begin
    Exec(strExecName, '-S', ExpandConstant('{app}\bin'), 0, ewWaitUntilTerminated, iResult);
  End;
End;

Procedure InitializeWizard;
Begin
  HttpdSettingsPage := CreateInputQueryPage(wpSelectTasks,
    'Select Master Server', 'Where is master server for web interface?',
    'Please enter host name or IP address of NetXMS master server. NetXMS web interface you are installing will provide connectivity to that server.');
  HttpdSettingsPage.Add('NetXMS server:', False);
  HttpdSettingsPage.Values[0] := GetPreviousData('MasterServer', 'localhost');
End;

Procedure RegisterPreviousData(PreviousDataKey: Integer);
Begin
  SetPreviousData(PreviousDataKey, 'MasterServer', HttpdSettingsPage.Values[0]);
End;

Function ShouldSkipPage(PageID: Integer): Boolean;
Begin
  If PageID = HttpdSettingsPage.ID Then
    Result := not IsComponentSelected('websrv')
  Else
    Result := False;
End;

Function GetMasterServer(Param: String): String;
Begin
  Result := HttpdSettingsPage.Values[0];
End;

