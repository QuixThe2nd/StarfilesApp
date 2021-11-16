#define MyAppName "Starfiles"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Anas Yousef"
#define MyAppURL "https://starfiles.co"
#define MyAppExeName "Starfiles.exe"

[Setup]
AppId={6A169FE9-2BA3-4097-9EEE-0F663048AA68}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\StarfilesApp
DisableProgramGroupPage=yes
LicenseFile=C:\Users\anas\GNU-General-Public-License-v3.0.txt
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=B:\Qt\Projects\Starfiles\build-x64
OutputBaseFilename=Starfiles-1.0.0-Beta-win64-Setup
SetupIconFile=B:\Qt\Projects\Starfiles\assets\images\logo\favicon.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\Starfiles.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\sqldrivers\*"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "B:\Qt\Projects\Starfiles\build-x64\Release\release\deploy\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

