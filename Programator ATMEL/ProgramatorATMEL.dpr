program ProgramatorATMEL;

uses
  Forms,
  Programator in 'Programator.pas' {MainForm},
  About in 'About.pas' {FormAbout},
  WriteReadVerify_Atmel in 'WriteReadVerify_Atmel.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'ProgATMEL';
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
