; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "OpenPrescription"
#define MyAppVersion "0.4"
#define MyAppPublisher "SalveoSoft"
#define MyAppURL "https://github.com/thefinalcutbg/OpenPrescription"
#define MyAppExeName "OpenPrescription.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B75E9463-F395-4918-BCE2-890029AE4224}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={commonpf64}/{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
UsePreviousAppDir=yes
LicenseFile=license.rtf
;InfoBeforeFile=InfoStart.txt
;InfoAfterFile=InforFinish.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=compiled
OutputBaseFilename=OpenPrescrSetup_win64
SetupIconFile=icon_prescr.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "bulgarian"; MessagesFile: "compiler:Languages\Bulgarian.isl"

[Dir]
Name: {app}; Permissions: users-full

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "files\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\vcredist_x64.EXE"; Parameters: "/q /norestart"; StatusMsg: "Installing VC++ redistributable 2010";
Filename: "{app}\VC_redist.x64.EXE"; Parameters: "/q /norestart"; StatusMsg: "Installing VC++ redistributable 2022";
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function IsUpgrade: Boolean;
var
    Value: string;
    UninstallKey: string;
begin
    UninstallKey := 'Software\Microsoft\Windows\CurrentVersion\Uninstall\' +
        ExpandConstant('{#SetupSetting("AppId")}') + '_is1';
    Result := (RegQueryStringValue(HKLM, UninstallKey, 'UninstallString', Value) or
        RegQueryStringValue(HKCU, UninstallKey, 'UninstallString', Value)) and (Value <> '');
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
if IsUpgrade then
  begin
  if PageID = wpLicense then
    Result := true
  end;
  begin
  if PageID = wpSelectTasks then
    Result := true
  end;
end;