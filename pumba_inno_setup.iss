[Setup]
AppName=PUMBA
AppVersion=0.1
DefaultDirName={autopf}\Pumba
DefaultGroupName=Pumba
OutputDir=bin
OutputBaseFilename=pumba_setup

[Files]
Source: "pumba.exe"; DestDir: "{app}"

[Icons]
Name: "{group}\Pumba"; Filename: "{app}\pumba.exe"
