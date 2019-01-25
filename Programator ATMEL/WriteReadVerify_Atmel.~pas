unit WriteReadVerify_ATMEL;

interface

uses
  Classes, Windows, SysUtils;

type
  TWrite_AT90USB = class(TThread)
  private
    { Private declarations }
  protected
    procedure Execute; override;
    constructor Create(CreateSuspended: Boolean);
    procedure AfiseazaLoad;
  end;

  TWrite_ATmega168 = class(TThread)
  private
    { Private declarations }
  protected
    procedure Execute; override;
    constructor Create(CreateSuspended: Boolean);
    procedure AfiseazaLoad;
  end;

  TWrite_AT90PWM3 = class(TThread)
  private
    { Private declarations }
  protected
    procedure Execute; override;
    constructor Create(CreateSuspended: Boolean);
    procedure AfiseazaLoad;
  end;

  TVerifyFlash_ATMEL = class(TThread)
  private
    { Private declarations }
  protected
    procedure Execute; override;
    constructor Create(CreateSuspended: Boolean);
    procedure AfiseazaLoad;
  end;

  TReadFlash_ATMEL = class(TThread)
  private
    { Private declarations }
  protected
    procedure Execute; override;
    constructor Create(CreateSuspended: Boolean);
    procedure AfiseazaLoad;
  end;

var
    Write_ATmega168:    TWrite_ATmega168;
    Write_AT90USB:      TWrite_AT90USB;
    Write_AT90PWM3:     TWrite_AT90PWM3;
    ReadFlash_ATMEL:    TReadFlash_ATMEL;
    VerifyFlash_ATMEL:  TVerifyFlash_ATMEL;

implementation

uses Programator;

//******************************************************************************
//******************************************************************************
//
//
//  Sectiune de procese de scriere memorie Flash
//  Subrutine WRITE in funtie de tipul de paginatie al muicrocontrolerului
//
//
//******************************************************************************
//******************************************************************************
constructor TWrite_AT90USB.Create(CreateSuspended: Boolean);
begin
FreeOnTerminate:=True;
inherited Create(CreateSuspended);
Priority:=tpNormal;
end;

procedure TWrite_AT90USB.AfiseazaLoad;
begin
MainForm.PB_Write.Position:=MainForm.PB_Write.Position+1;
end;

constructor TWrite_ATmega168.Create(CreateSuspended: Boolean);
begin
FreeOnTerminate:=True;
inherited Create(CreateSuspended);
Priority:=tpNormal;
end;

procedure TWrite_ATmega168.AfiseazaLoad;
begin
MainForm.PB_Write.Position:=MainForm.PB_Write.Position+1;
end;

constructor TWrite_AT90PWM3.Create(CreateSuspended: Boolean);
begin
FreeOnTerminate:=True;
inherited Create(CreateSuspended);
Priority:=tpNormal;
end;

procedure TWrite_AT90PWM3.AfiseazaLoad;
begin
MainForm.PB_Write.Position:=MainForm.PB_Write.Position+1;
end;

procedure TWrite_AT90USB.Execute;
var
   ContorRanduri, ContorIntarziere: Integer;
   Rand: string;
   ADRESA: DWORD;
   DATALOW, DATAHIGH: Byte;
   BackPage, ActualPage, BackExtendedPage, ActualExtendedPage: Word;
begin
WriteSauNu:=True;

    Rand := bufferProgramFlash.Strings[0];
    ADRESA := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                                     256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));
    BackPage := Word(ADRESA and $0000FF80);
    BackExtendedPage:=Word((ADRESA and $00FF0000) shr 16);

    DATA_OUT[1] := LOAD_EXTENDED_ADDRESS_BYTE_1;
    DATA_OUT[2] := LOAD_EXTENDED_ADDRESS_BYTE_2;
    DATA_OUT[3] := ActualExtendedPage;
    DATA_OUT[4] := 0;
    ReadWritePORT;
    for ContorIntarziere:=1 to 25000*Intarziere_1us do;// intarziere la scriere 25ms
    
for ContorRanduri:=0 to bufferProgramFlash.Count-1 do begin
    Rand := bufferProgramFlash.Strings[ContorRanduri];
    ADRESA := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                                     256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));

    DATAHIGH := 16*CharToInt(Rand[8]) + CharToInt(Rand[9]);
    DATALOW := 16*CharToInt(Rand[10]) + CharToInt(Rand[11]);

    ActualExtendedPage:=Word((ADRESA and $00FF0000) shr 16);
    if (ActualExtendedPage <> BackExtendedPage) then begin
        DATA_OUT[1] := LOAD_EXTENDED_ADDRESS_BYTE_1;
        DATA_OUT[2] := LOAD_EXTENDED_ADDRESS_BYTE_2;
        DATA_OUT[3] := Byte(ActualExtendedPage);
        DATA_OUT[4] := 0;
        ReadWritePORT;
        for ContorIntarziere:=1 to 25000*Intarziere_1us do;// intarziere la scriere 25ms
        BackExtendedPage := ActualExtendedPage;
    end;
    ActualPage := Word(ADRESA and $0000FF80);
    if (ActualPage <> BackPage) then begin
        DATA_OUT[1] := WRITE_PAGE_FLASH;
        DATA_OUT[2] := Byte(BackPage shr 8);
        DATA_OUT[3] := Byte(BackPage and $0080);
        DATA_OUT[4] := 0;
        ReadWritePORT;
        for ContorIntarziere:=1 to 25000*Intarziere_1us do;// intarziere la scriere 25ms
        BackPage := ActualPage;
    end;

    DATA_OUT[1] := LOAD_FLASH_LOW;
    DATA_OUT[2] := 0;
    DATA_OUT[3] := Byte(ADRESA and $0000007F);
    DATA_OUT[4] := DATALOW;
    ReadWritePORT;
    DATA_OUT[1] := LOAD_FLASH_HIGH;
    DATA_OUT[2] := 0;
    DATA_OUT[3] := Byte(ADRESA and $0000007F);
    DATA_OUT[4] := DATAHIGH;
    ReadWritePORT;
    Synchronize(AfiseazaLoad);
end;
DATA_OUT[1] := WRITE_PAGE_FLASH;
DATA_OUT[2] := Byte(ActualPage shr 8);
DATA_OUT[3] := Byte(ActualPage and $0080);
DATA_OUT[4] := 0;
ReadWritePORT;
for ContorIntarziere:=1 to 25000*Intarziere_1us do;     // intarziere la scriere 25ms
end;


procedure TWrite_ATmega168.Execute;
var
   ContorRanduri, ContorIntarziere: Integer;
   Rand: string;
   ADRESA: DWORD;
   DATALOW, DATAHIGH: Byte;
   BackPage, ActualPage: Word;
begin
WriteSauNu:=True;

    Rand := bufferProgramFlash.Strings[0];
    ADRESA := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                                     256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));
    BackPage := Word(ADRESA and $0000FF80);
for ContorRanduri:=0 to bufferProgramFlash.Count-1 do begin
    Rand := bufferProgramFlash.Strings[ContorRanduri];
    ADRESA := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                                     256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));

    DATAHIGH := 16*CharToInt(Rand[8]) + CharToInt(Rand[9]);
    DATALOW := 16*CharToInt(Rand[10]) + CharToInt(Rand[11]);

    ActualPage := Word(ADRESA and $0000FFC0);
    if (ActualPage <> BackPage) then begin
        DATA_OUT[1] := WRITE_PAGE_FLASH;
        DATA_OUT[2] := Byte(BackPage shr 8);
        DATA_OUT[3] := Byte(BackPage and $00C0);
        DATA_OUT[4] := 0;
        ReadWritePORT;
        for ContorIntarziere:=1 to 25000*Intarziere_1us do;// intarziere la scriere 25ms
        BackPage := ActualPage;
    end;

    DATA_OUT[1] := LOAD_FLASH_LOW;
    DATA_OUT[2] := 0;
    DATA_OUT[3] := Byte(ADRESA and $0000003F);
    DATA_OUT[4] := DATALOW;
    ReadWritePORT;
    DATA_OUT[1] := LOAD_FLASH_HIGH;
    DATA_OUT[2] := 0;
    DATA_OUT[3] := Byte(ADRESA and $0000003F);
    DATA_OUT[4] := DATAHIGH;
    ReadWritePORT;
    Synchronize(AfiseazaLoad);
end;
DATA_OUT[1] := WRITE_PAGE_FLASH;
DATA_OUT[2] := Byte(ActualPage shr 8);
DATA_OUT[3] := Byte(ActualPage and $00C0);
DATA_OUT[4] := 0;
ReadWritePORT;
for ContorIntarziere:=1 to 25000*Intarziere_1us do;     // intarziere la scriere 25ms
end;
//******************************************************************************
//
//  Write AT90PWM3
//
//******************************************************************************
procedure TWrite_AT90PWM3.Execute;
var
   ContorRanduri, ContorIntarziere: Integer;
   Rand: string;
   ADRESA: DWORD;
   DATALOW, DATAHIGH: Byte;
   BackPage, ActualPage: Word;
begin
WriteSauNu:=True;

    Rand := bufferProgramFlash.Strings[0];
    ADRESA := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                  256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));
    BackPage := Word(ADRESA and $0000FFE0);
for ContorRanduri:=0 to bufferProgramFlash.Count-1 do begin
    Rand := bufferProgramFlash.Strings[ContorRanduri];
    ADRESA := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                  256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));

    DATAHIGH := 16*CharToInt(Rand[8]) + CharToInt(Rand[9]);
    DATALOW := 16*CharToInt(Rand[10]) + CharToInt(Rand[11]);

    ActualPage := Word(ADRESA and $0000FFC0);
    if (ActualPage <> BackPage) then begin
        DATA_OUT[1] := WRITE_PAGE_FLASH;
        DATA_OUT[2] := Byte((BackPage shr 8) and $001F);   //limitarea la 8KWords - 16 Kocteti
        DATA_OUT[3] := Byte(BackPage);
        DATA_OUT[4] := 0;
        ReadWritePORT;
        for ContorIntarziere:=1 to 25000*Intarziere_1us do;// intarziere la scriere 25ms
        BackPage := ActualPage;
    end;

    DATA_OUT[1] := LOAD_FLASH_LOW;
    DATA_OUT[2] := 0;
    DATA_OUT[3] := Byte(ADRESA and $0000003F);
    DATA_OUT[4] := DATALOW;
    ReadWritePORT;
    DATA_OUT[1] := LOAD_FLASH_HIGH;
    DATA_OUT[2] := 0;
    DATA_OUT[3] := Byte(ADRESA and $0000003F);
    DATA_OUT[4] := DATAHIGH;
    ReadWritePORT;
    Synchronize(AfiseazaLoad);
end;
DATA_OUT[1] := WRITE_PAGE_FLASH;
DATA_OUT[2] := Byte(ActualPage shr 8);
DATA_OUT[3] := Byte(ActualPage and $00E0);
DATA_OUT[4] := 0;
ReadWritePORT;
for ContorIntarziere:=1 to 25000*Intarziere_1us do;     // intarziere la scriere 25ms
end;


//******************************************************************************
//******************************************************************************
//
//
//  Sectiune proces de citire memorie Flash
//
//
//
//******************************************************************************
//******************************************************************************

constructor TReadFlash_ATMEL.Create(CreateSuspended: Boolean);
begin
FreeOnTerminate:=True;
inherited Create(CreateSuspended);
Priority:=tpNormal;
end;

procedure TReadFlash_ATMEL.AfiseazaLoad;
begin
    MainForm.PB_Write.Position:=MainForm.PB_Write.Position+1;
end;


procedure TReadFlash_ATMEL.Execute;
var
   Celula, Cod_ASCII: String;
   J,K: Integer;
   Pagina,DATA_HIGH, DATA_LOW: Word;
   Adresa: Byte;
   Eroare: Bool;
begin
Cod_ASCII:='';
if (MainForm.LV_Flash.Items.Count<>0) then MainForm.LV_Flash.Items.Clear;
Eroare:=False;
J:=0;
K:=0;
Celula:='';
for Pagina:=(Adresa_Start_Flash shr 4) to ((Adresa_Stop_Flash shr 4)-1) do begin
    MainForm.LV_Flash.Items.Add;
    MainForm.LV_Flash.Items[J].Caption:=IntToHex((Pagina shl 4),4);
    for Adresa:=0 to 15 do begin          //MaxLengthFlash-1
        DATA_OUT[1] := READ_FLASH_HIGH;
        DATA_OUT[2] := Pagina shr 4;
        DATA_OUT[3] := ((Pagina shl 4)+Adresa) and $00FF;
        DATA_OUT[4] := 255;
        ReadWritePORT;
        if DATA_IN[2] <> DATA_OUT[1] then begin Eroare:=True;Break;end;
        DATA_HIGH:=DATA_IN[4];

        DATA_OUT[1] := READ_FLASH_LOW;
        DATA_OUT[2] := Pagina shr 4;
        DATA_OUT[3] := ((Pagina shl 4)+Adresa) and $00FF;
        DATA_OUT[4] := 255;
        ReadWritePORT;
        if DATA_IN[2] <> DATA_OUT[1] then begin Eroare:=True;Break;end;
        DATA_LOW:=DATA_IN[4];

        Celula := IntToHex(((DATA_HIGH shl 8) or DATA_LOW),4);
        Cod_ASCII:=Cod_ASCII+':'+Char(DATA_LOW)+' ';

        if (Adresa=8) then begin
            Inc(J);
            K:=0;
            MainForm.LV_Flash.Items.Add;
            MainForm.LV_Flash.Items[J].Caption:=IntToHex(((Pagina shl 4) or $08),4);
            MainForm.LV_Flash.Items[J].SubItems.Add('');
            MainForm.LV_Flash.Items[J].SubItems.Strings[K]:=Celula;
        end
        else begin
            MainForm.LV_Flash.Items[J].SubItems.Add('');
            MainForm.LV_Flash.Items[J].SubItems.Strings[K]:=Celula;
        end;
        Inc(K);
        if (Adresa=15)then begin
            MainForm.LV_Flash.Items[J].SubItems.Add('');
            MainForm.LV_Flash.Items[J].SubItems.Strings[K]:=Cod_ASCII;
            Cod_ASCII:='';
            Inc(J);
            K:=0;
        end;
        if (Adresa=7) then begin
            MainForm.LV_Flash.Items[J].SubItems.Add('');
            MainForm.LV_Flash.Items[J].SubItems.Strings[K]:=Cod_ASCII;
            Cod_ASCII:='';
        end;
        Synchronize(AfiseazaLoad);
    end;
end;
if Eroare then begin    MessageBox(0,PChar('Eroare de citire din Flash!'+#13#10+
                                        'La adresa ['+IntToHex(((Pagina shl 4)+Adresa),4)+']'+#13#10+
                                        'Probabil dispozitiv deconectat'),'Read Flash', MB_OK or MB_ICONERROR);
                        MainForm.PB_Write.Position:=MainForm.PB_Write.Min;
end
else begin  MessageBox(0,PChar( 'Sectiunea de memorie ['+IntToHex(Adresa_Start_Flash,5)+'H]-['+IntToHex(Adresa_Stop_Flash,5)+'H]'+#13#10+
                                'a fost citita !'),'Read Flash', MB_OK);
            MainForm.PB_Write.Position:=MainForm.PB_Write.Min;
end;
end;

//******************************************************************************
//******************************************************************************
//
//
//  Proces verificare memorie Flash
//
//
//
//******************************************************************************
//******************************************************************************
constructor TVerifyFlash_ATMEL.Create(CreateSuspended: Boolean);
begin
FreeOnTerminate:=True;
inherited Create(CreateSuspended);
Priority:=tpNormal;
end;

procedure TVerifyFlash_ATMEL.AfiseazaLoad;
begin
MainForm.PB_Write.Position:=MainForm.PB_Write.Position+1;
end;

procedure TVerifyFlash_ATMEL.Execute;
var
   ContorRanduri, EroareAdresa: integer;
   Rand: string[11];
   ADRESA: array[1..2] of byte;
   DATE: array[1..2] of byte;
   Corect: Boolean;
begin
Corect:=True;
for ContorRanduri:=0 to bufferProgramFlash.Count-1 do begin
    Rand := bufferProgramFlash.Strings[ContorRanduri];
    ADRESA[1] := (CharToInt(Rand[3]) shl 4) or CharToint(Rand[4]);
    ADRESA[2] := (CharToInt(Rand[5]) shl 4) or CharToint(Rand[6]);
    DATE[1] := (CharToInt(Rand[8]) shl 4) or CharToint(Rand[9]);
    DATE[2] := (CharToInt(Rand[10]) shl 4) or CharToint(Rand[11]);

    DATA_OUT[1] := READ_FLASH_HIGH;
    DATA_OUT[2] := ADRESA[1];
    DATA_OUT[3] := ADRESA[2];
    DATA_OUT[4] := 255;
    ReadWritePORT;                                          // citire
    if DATE[1] <> DATA_IN[4] then begin Corect := False;Break;end;
    DATA_OUT[1] := READ_FLASH_LOW;
    DATA_OUT[2] := ADRESA[1];
    DATA_OUT[3] := ADRESA[2];
    DATA_OUT[4] := 255;
    ReadWritePORT;                                          // citire
    if DATE[2] <> DATA_IN[4] then begin Corect := False;Break;end;
Synchronize(AfiseazaLoad);
end;
if not Corect then begin
    EroareAdresa := 4096*(ADRESA[1] shr 4)+256*(ADRESA[1] and 15)+16*(ADRESA[2] shr 4)+(ADRESA[2] and 15);
    MessageBox(0,PChar( 'Incompatibilitate la adresa '+IntToHex(EroareAdresa,4)+'H'+#13#10+
                        'intre programul din Flash si Fisier !'),'Verify Flash', MB_OK or MB_ICONERROR);
    MainForm.PB_Write.Position:=MainForm.PB_Write.Min;
end
else begin
    MessageBox(0,PChar( 'Programul din memoria Flash'+#13#10+
                        'este compatibil cu programul din Fisier !'),'Verify Flash', MB_OK);
    MainForm.PB_Write.Position:=MainForm.PB_Write.Min;
end;
end;


end.
