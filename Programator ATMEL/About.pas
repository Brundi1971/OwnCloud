//about
unit About;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TFormAbout = class(TForm)
    OKButton: TButton;
    Label1: TLabel;
    Label3: TLabel;
    Label4: TLabel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

procedure ShowAbout;

var
  FormAbout: TFormAbout;

implementation

{$R *.DFM}

procedure ShowAbout;
begin
     with TFormAbout.Create(Application) do
     try
     ShowModal;
    finally
      Free;
    end;
end;
end.
