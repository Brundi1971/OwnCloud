unit Programator;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, Tabs, Menus, ComCtrls, Math, ImgList;

type
  TMainForm = class(TForm)
    TabSet: TTabSet;
    MMenu: TMainMenu;
    Ruleaza: TMenuItem;
    About: TMenuItem;
    OpenFileDialog: TOpenDialog;
    OpenFileDialogEE: TOpenDialog;
    Bevel5: TBevel;
    OpenSPI: TMenuItem;
    ChipErase: TMenuItem;
    StatusBar1: TStatusBar;
    PB_Write: TProgressBar;
    CB_TipAT: TComboBox;
    TB_Time: TTrackBar;
    L_Position: TLabel;
    L_ConstantaTimp: TLabel;
    L_Cicluri_EraseWrite: TLabel;
    TimerTest: TTimer;
    IL_led: TImageList;
    LED_RESET: TImage;
    LED_CLK: TImage;
    LED_DATA_OUT: TImage;
    LED_DATA_IN: TImage;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Bevel2: TBevel;
    B_Test: TSpeedButton;
    Notebook: TNotebook;
    B_OpenFileFlash: TSpeedButton;
    B_WriteFlash: TSpeedButton;
    B_VerifyFlash: TSpeedButton;
    B_ReadFlash: TSpeedButton;
    L_NrOcteti_FileFlash: TLabel;
    L_AdresaStart: TLabel;
    L_AdresaStop: TLabel;
    L_NoFlashRead: TLabel;
    S_MemorieFlash: TShape;
    S_MemorieFlashRead: TShape;
    E_FileFlash: TEdit;
    LV_Flash: TListView;
    B_OpenFileEEPROM: TSpeedButton;
    B_WriteEEPROMFromList: TSpeedButton;
    B_WriteEEPROM: TSpeedButton;
    B_VerifyEEPROM: TSpeedButton;
    L_NrOcteti_FileEE: TLabel;
    B_ReadEEPROM: TSpeedButton;
    E_OpenFileEEPROM: TEdit;
    LV_EE: TListView;
    LabelMesajEroare: TLabel;
    B_ReadFuse_ATtiny2313: TSpeedButton;
    B_WriteFuse_ATtiny2313: TSpeedButton;
    GB_ExtendedFuseByte_ATtiny2313: TGroupBox;
    CB_SELFPRGEN_ATtiny2313: TCheckBox;
    GB_FuseHighByte_ATtiny2313: TGroupBox;
    CB_RSTDISBL_ATtiny2313: TCheckBox;
    CB_BODLEVEL0_ATtiny2313: TCheckBox;
    CB_BODLEVEL1_ATtiny2313: TCheckBox;
    CB_BODLEVEL2_ATtiny2313: TCheckBox;
    CB_WDTON_ATtiny2313: TCheckBox;
    CB_SPIEN_ATtiny2313: TCheckBox;
    CB_EESAVE_ATtiny2313: TCheckBox;
    CB_DWEN_ATtiny2313: TCheckBox;
    GB_FuseLowByte_ATtiny2313: TGroupBox;
    CB_CKDIV8_ATtiny2313: TCheckBox;
    CB_CKOUT_ATtiny2313: TCheckBox;
    CB_SUT1_ATtiny2313: TCheckBox;
    CB_SUT0_ATtiny2313: TCheckBox;
    CB_CKSEL3_ATtiny2313: TCheckBox;
    CB_CKSEL2_ATtiny2313: TCheckBox;
    CB_CKSEL1_ATtiny2313: TCheckBox;
    CB_CKSEL0_ATtiny2313: TCheckBox;
    SB_ATtiny2313: TScrollBar;
    GB_LockBitByte_ATtiny2313: TGroupBox;
    CB_LOCK1_ATtiny2313: TCheckBox;
    CB_LOCK2_ATtiny2313: TCheckBox;
    GB_LockBitByte_ATmega168: TGroupBox;
    CB_LOCK1_ATmega168: TCheckBox;
    CB_LOCK2_ATmega168: TCheckBox;
    CB_BLB01_ATmega168: TCheckBox;
    CB_BLB11_ATmega168: TCheckBox;
    CB_BLB12_ATmega168: TCheckBox;
    CB_BLB02_ATmega168: TCheckBox;
    GB_FuseHighByte_ATmega168: TGroupBox;
    CB_BODLEVEL0_ATmega168: TCheckBox;
    CB_BODLEVEL1_ATmega168: TCheckBox;
    CB_BODLEVEL2_ATmega168: TCheckBox;
    CB_EESAVE_ATmega168: TCheckBox;
    CB_WDTON_ATmega168: TCheckBox;
    CB_SPIEN_ATmega168: TCheckBox;
    CB_DWEN_ATmega168: TCheckBox;
    CB_RSTDISBL_ATmega168: TCheckBox;
    GB_FuseLowByte_ATmega168: TGroupBox;
    CB_CKSEL0_ATmega168: TCheckBox;
    CB_CKSEL1_ATmega168: TCheckBox;
    CB_CKSEL2_ATmega168: TCheckBox;
    CB_CKSEL3_ATmega168: TCheckBox;
    CB_SUT0_ATmega168: TCheckBox;
    CB_SUT1_ATmega168: TCheckBox;
    CB_CKOUT_ATmega168: TCheckBox;
    CB_CKDIV8_ATmega168: TCheckBox;
    B_WriteFuseATmega168: TButton;
    B_ReadFuseATmega168: TButton;
    GB_ExtendedFuseByte_ATmega168: TGroupBox;
    CB_BOOTRST_ATmega168: TCheckBox;
    CB_BOOTSZ0_ATmega168: TCheckBox;
    CB_BOOTSZ1_ATmega168: TCheckBox;
    SB_ATmega168: TScrollBar;
    GB_LockBitByte_AT90USB: TGroupBox;
    CB_LOCK1_AT90USB: TCheckBox;
    CB_LOCK2_AT90USB: TCheckBox;
    CB_BLB01_AT90USB: TCheckBox;
    CB_BLB02_AT90USB: TCheckBox;
    CB_BLB11_AT90USB: TCheckBox;
    CB_BLB12_AT90USB: TCheckBox;
    GB_ExtendedFuseByte_AT90USB: TGroupBox;
    CB_BODLEVEL0_AT90USB: TCheckBox;
    CB_BODLEVEL1_AT90USB: TCheckBox;
    CB_BODLEVEL2_AT90USB: TCheckBox;
    CB_HWBE_AT90USB: TCheckBox;
    GB_FuseHighByte_AT90USB: TGroupBox;
    CB_BOOTRST_AT90USB: TCheckBox;
    CB_BOOTSZ0_AT90USB: TCheckBox;
    CB_BOOTSZ1_AT90USB: TCheckBox;
    CB_EESAVE_AT90USB: TCheckBox;
    CB_WDTON_AT90USB: TCheckBox;
    CB_SPIEN_AT90USB: TCheckBox;
    CB_JTAGEN_AT90USB: TCheckBox;
    CB_OCDEN_AT90USB: TCheckBox;
    GB_FuseLowByte_AT90USB: TGroupBox;
    CB_CKSEL0_AT90USB: TCheckBox;
    CB_CKSEL1_AT90USB: TCheckBox;
    CB_CKSEL2_AT90USB: TCheckBox;
    CB_CKSEL3_AT90USB: TCheckBox;
    CB_SUT0_AT90USB: TCheckBox;
    CB_SUT1_AT90USB: TCheckBox;
    CB_CKOUT_AT90USB: TCheckBox;
    CB_CKDIV8_AT90USB: TCheckBox;
    SB_AT90USB: TScrollBar;
    B_ReadFuseAT90USB: TButton;
    B_WriteFuseAT90USB: TButton;
    GB_LockBitByte_AT90PWM: TGroupBox;
    CB_LOCK1_AT90PWM: TCheckBox;
    CB_LOCK2_AT90PWM: TCheckBox;
    CB_BLB01_AT90PWM: TCheckBox;
    CB_BLB02_AT90PWM: TCheckBox;
    CB_BLB11_AT90PWM: TCheckBox;
    CB_BLB12_AT90PWM: TCheckBox;
    GB_ExtendedFuseByte_AT90PWM: TGroupBox;
    CB_BOOTRST_AT90PWM: TCheckBox;
    CB_BOOTSZ0_AT90PWM: TCheckBox;
    CB_BOOTSZ1_AT90PWM: TCheckBox;
    CB_PSCRV_AT90PWM: TCheckBox;
    CB_PSC0RB_AT90PWM: TCheckBox;
    CB_PSC1RB_AT90PWM: TCheckBox;
    CB_PSC2RB_AT90PWM: TCheckBox;
    GB_FuseHighByte_AT90PWM: TGroupBox;
    CB_BODLEVEL0_AT90PWM: TCheckBox;
    CB_BODLEVEL1_AT90PWM: TCheckBox;
    CB_BODLEVEL2_AT90PWM: TCheckBox;
    CB_EESAVE_AT90PWM: TCheckBox;
    CB_WDTON_AT90PWM: TCheckBox;
    CB_SPIEN_AT90PWM: TCheckBox;
    CB_DWEN_AT90PWM: TCheckBox;
    CB_RSTDISBL_AT90PWM: TCheckBox;
    GB_FuseLowByte_AT90PWM: TGroupBox;
    CB_CKSEL0_AT90PWM: TCheckBox;
    CB_CKSEL1_AT90PWM: TCheckBox;
    CB_CKSEL2_AT90PWM: TCheckBox;
    CB_CKSEL3_AT90PWM: TCheckBox;
    CB_SUT0_AT90PWM: TCheckBox;
    CB_SUT1_AT90PWM: TCheckBox;
    CB_CKOUT_AT90PWM: TCheckBox;
    CB_CKDIV8_AT90PWM: TCheckBox;
    SB_AT90PWM: TScrollBar;
    B_ReadFuseAT90PWM: TButton;
    B_WriteFuseAT90PWM: TButton;
    procedure FormCreate(Sender: TObject);
    procedure AboutClick(Sender: TObject);
    procedure RuleazaClick(Sender: TObject);
    procedure B_OpenFileFlashClick(Sender: TObject);
    procedure B_OpenFileEEPROMClick(Sender: TObject);
    procedure B_WriteFlashClick(Sender: TObject);
    procedure B_WriteEEPROMClick(Sender: TObject);
    procedure B_VerifyFlashClick(Sender: TObject);
    procedure B_VerifyEEPROMClick(Sender: TObject);
    procedure B_ReadFlashClick(Sender: TObject);
    procedure B_WriteEEPROMFromListClick(Sender: TObject);
    procedure S_MemorieFlashReadMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure S_MemorieFlashReadMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure S_MemorieFlashReadMouseMove(Sender: TObject;
      Shift: TShiftState; X, Y: Integer);
    procedure B_ReadEEPROMClick(Sender: TObject);
    procedure B_ReadFuse_ATtiny2313Click(Sender: TObject);
    procedure B_WriteFuse_ATtiny2313Click(Sender: TObject);
    procedure B_WriteFuseATmega168Click(Sender: TObject);
    procedure B_ReadFuseATmega168Click(Sender: TObject);
    procedure SB_ATmega168Change(Sender: TObject);
    procedure OpenSPIClick(Sender: TObject);
    procedure ChipEraseClick(Sender: TObject);
    procedure TabSetChange(Sender: TObject; NewTab: Integer;
      var AllowChange: Boolean);
    procedure SB_ATtiny2313Change(Sender: TObject);
    procedure LV_EEEditing(Sender: TObject; Item: TListItem;
      var AllowEdit: Boolean);
    procedure LV_EEChanging(Sender: TObject; Item: TListItem;
      Change: TItemChange; var AllowChange: Boolean);
    procedure LV_EECustomDrawSubItem(Sender: TCustomListView;
      Item: TListItem; SubItem: Integer; State: TCustomDrawState;
      var DefaultDraw: Boolean);
    procedure B_ReadFuseAT90USBClick(Sender: TObject);
    procedure B_WriteFuseAT90USBClick(Sender: TObject);
    procedure SB_AT90USBChange(Sender: TObject);
    procedure TB_TimeChange(Sender: TObject);
    procedure CB_TipATChange(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure B_ReadFuseAT90PWMClick(Sender: TObject);
    procedure B_WriteFuseAT90PWMClick(Sender: TObject);
    procedure SB_AT90PWMChange(Sender: TObject);
    procedure B_TestClick(Sender: TObject);
    procedure TimerTestTimer(Sender: TObject);
  private
    procedure TerminaProces(Sender: TObject);
  public
    { Public declarations }
  end;

    procedure ReadWritePORT;
    function  CharToInt(Caracter: Char): Byte;
    function ReadPort(AdresaPort: Integer):Byte; stdcall; external 'inpout32.dll' name 'Inp32';
    procedure WritePort(AdresaPort: Integer; Value:Byte); stdcall; external 'inpout32.dll' name 'Out32';
    procedure transfHexInGen;

type
  Allocation = record
    Name:         array[0..7] of String;
    ExistOrNot:   array of Boolean;
end;

type
  Fuse = record
    Name: String;
    LockBitsByte:      Allocation;
    ExtendedFuseByte:  Allocation;
    HighFuseByte:      Allocation;
    LowFuseByte:       Allocation;
end;


const

       // adresa portului paralel
       PORT_DATE                            = 888;
       PORT_STARE                           = 889;
       PORT_CONTROL                         = 890;

       // codurile instructiunilor pentru programarea procesorului ATMEL
       // ENABLE SPI
       SPI_ENABLE_BYTE_1                    = $AC;
       SPI_ENABLE_BYTE_2                    = $53;

       // ERASE FLASH
       CHIP_ERASE_BYTE_1                    = $AC;
       CHIP_ERASE_BYTE_2                    = $80;

       // READ FLASH
       READ_FLASH_LOW                       = $20;
       READ_FLASH_HIGH                      = $28;
       //******************************************************

       WRITE_PAGE_FLASH                     = $4c;
       LOAD_FLASH_LOW                       = $40;
       LOAD_FLASH_HIGH                      = $48;
       LOAD_EXTENDED_ADDRESS_BYTE_1         = $4d;
       LOAD_EXTENDED_ADDRESS_BYTE_2         = $00;

       WRITE_EEPROM                         = $C0;
       READ_EEPROM                          = $A0;


       RESET_ON                             = $02;
       RESET_OFF                            = $FD;
       CLK_ON                               = $01;
       CLK_OFF                              = $FE;
       DATA_OUT_ON                          = $80;
       DATA_OUT_OFF                         = $7F;

       MinLengthFlashDefault                = 1028;
       MinLengthEEDefault                   = 128;
       MaxLengthFlashDefault                = 65536;
       MaxLengthEEDefault                   = 512;
       MascaAlimentare                      = 124;

       Intarziere_1usDefault=100;
       FileInformation='Prog.ini';

       LED_RESET_ON                         = 0;
       LED_RESET_OFF                        = 1;
       LED_CLK_ON                           = 2;
       LED_CLK_OFF                          = 3;
       LED_DATA_OUT_ON                      = 4;
       LED_DATA_OUT_OFF                     = 5;

var
        MainForm: TMainForm;

        bufferProgramFlash: TStringList;
        bufferProgramEE: TStringList;
        bufferProvizoriu: TStringList;
        bufferEE: array[0..MaxLengthEEDefault-1] of Byte;
        bufferEEOld: array[0..MaxLengthEEDefault-1] of Byte;
        ModificatEE: array[0..MaxLengthEEDefault-1] of Bool;

        DATA_OUT: array[1..4] of Byte;
        DATA_IN: array[1..4] of Byte;

        Index, EroareAdresa: Integer;
        ModificatSauNu, WriteSauNu, SDragging, SSize, AbortSauNu, EEPROMCitit, ReadFileEE: Bool;
        MaxLengthFlash, MaxLengthEE, ConstantaTimp, Intarziere_1us, Contor_EraseWrite, Index_MemorieRead: Integer;
        ExtendedFuseBitsByte, HighFuseBitsByte, FuseBitsByte: Byte;
        DefaultReadFlash, DefaultReadEE: Integer;

        ExtensieFile: String;
        EroareOpenSPI: Boolean;

        Adresa_Start_Flash, Adresa_Stop_Flash: Integer;

        Toggle_LED_TEST: Integer;


implementation

{$R *.DFM}
{$H-}

uses About, WriteReadVerify_ATMEL;


procedure TMainForm.FormCreate(Sender: TObject);
var
   OctetOUT: Byte;
   PathFile: array[0..MAX_PATH-1] of Char;
   NameFile, ValoareString: String;
   FileSetari: TStringList;
   TipMicrocontroler: String;
   i: Integer;
   Bitmap: TBitmap;
begin
ReadFileEE:=False;
FileSetari:=TStringlist.Create;
GetModuleFileName(hInstance, PathFile, SizeOf(PathFile));
NameFile:=ExtractFilePath(PathFile)+FileInformation;
LabelMesajEroare.Caption:='';

try
   FileSetari.LoadFromFile(NameFile);

   TipMicrocontroler:=FileSetari.Values['TipMicrocontroler'];
   for i:=0 to CB_TipAT.Items.Count-1 do begin
        if (FileSetari.Values['TipMicrocontroler'] = CB_TipAT.Items[i]) then CB_TipAT.ItemIndex:=i;
   end;
   TB_Time.Position:= StrToInt (FileSetari.Values['ConstantaTimp']);
   ConstantaTimp := TB_Time.Position;
   Contor_EraseWrite := StrToInt(FileSetari.Values['Contor_EraseWrite']);
   L_Cicluri_EraseWrite.Caption:='Cicluri Erase/Write:  '+IntToStr(Contor_EraseWrite);
except
   on EFOpenError do begin
   CB_TipAT.ItemIndex:=0;
   ConstantaTimp:=1;
   Intarziere_1us:=ConstantaTimp*Intarziere_1usDefault;
   Contor_EraseWrite:=0;
   end;
end;

if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATtiny2313') then begin  MaxLengthFlash:=1024;MaxLengthEE:=128;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='AT90USB1287') then begin  MaxLengthFlash:=65536;MaxLengthEE:=4096;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATmega168') then begin  MaxLengthFlash:=8192;MaxLengthEE:=512;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATmega256RZAV') then begin  MaxLengthFlash:=131072;MaxLengthEE:=4096;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='AT90PWM3') then begin  MaxLengthFlash:=4096;MaxLengthEE:=512;end;

bufferProgramFlash:=TStringList.Create;
bufferProgramEE:=TStringList.Create;
bufferProvizoriu:=TStringList.Create;

L_NrOcteti_FileFlash.Visible:=False;
L_NrOcteti_FileEE.Visible:=False;

Intarziere_1us:=ConstantaTimp*Intarziere_1usDefault;

EEPROMCitit:=False;

OctetOUT := MascaAlimentare;
WritePort(PORT_DATE,OctetOUT); // Reset device

MMenu.Items[1].Enabled := False;

SB_ATmega168Change(Self);
SB_ATtiny2313Change(Self);
SB_AT90USBChange(Self);

B_ReadFlash.Enabled:=False;
B_ReadEEPROM.Enabled:=False;
EroareOpenSPI:=True;

PB_Write.Height := StatusBar1.Height-2;
PB_Write.Top := StatusBar1.Top+2;
PB_Write.Left := StatusBar1.Left + StatusBar1.Width - PB_Write.Width;

if (MaxLengthFlash>=32768) then begin
    Adresa_Start_Flash:=0;
    DefaultReadFlash:=4096;
    Adresa_Stop_Flash:=DefaultReadFlash;
    S_MemorieFlashRead.Top:=S_MemorieFlash.Top;
    S_MemorieFlashRead.Height:= trunc(256/trunc(MaxLengthFlash/DefaultReadFlash));
end
else if ((MaxLengthFlash>=2048) and (MaxLengthFlash<=16384)) then begin
    Adresa_Start_Flash:=0;
    DefaultReadFlash:=512;
    Adresa_Stop_Flash:=DefaultReadFlash;
    S_MemorieFlashRead.Top:=S_MemorieFlash.Top;
    S_MemorieFlashRead.Height:= trunc(256/trunc(MaxLengthFlash/DefaultReadFlash));
end
else begin
    Adresa_Start_Flash:=0;
    DefaultReadFlash:=64;
    Adresa_Stop_Flash:=DefaultReadFlash;
    S_MemorieFlashRead.Top:=S_MemorieFlash.Top;
    S_MemorieFlashRead.Height:= trunc(256/trunc(MaxLengthFlash/DefaultReadFlash));
end;
L_AdresaStart.Caption:=IntToHex(Adresa_Start_Flash,5)+'H';
L_AdresaStop.Caption:=IntToHex(Adresa_Stop_Flash-1,5)+'H';
L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash - Adresa_Start_Flash)+' Words';
L_AdresaStart.Top := S_MemorieFlashRead.Top-12;
L_AdresaStop.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height+4;
L_NoFlashRead.Top := S_MemorieFlashRead.Top + trunc(S_MemorieFlashRead.Height/2)-4;
L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash-Adresa_Start_Flash)+': Words';

end;


procedure TMainForm.AboutClick(Sender: TObject);
begin
     ShowAbout;
end;

    { Trateaza buton OpenFileFlash }

procedure TMainForm.B_OpenFileFlashClick(Sender: TObject);
var
   ContorRand, AdresaEronata, AdresaSegment,ContorCampRand, ByteCount: Integer;
   FormatCorect, AdresaCorecta, CRCCorect: Bool;
   Rand: String;
   NumeFisierFlash: AnsiString;
   tempBuff0,tempBuff1,tempBuff2: Byte;
begin
FormatCorect:=True;
AdresaCorecta:=True;
CRCCorect:=True;
AdresaSegment:=0;
if OpenFileDialog.Execute then begin
     OpenFileDialog.InitialDir := ExtractFilePath(OpenFileDialog.FileName);
     NumeFisierFlash := ExtractFilePath(OpenFileDialog.Filename)+ExtractFileName(OpenFileDialog.Filename);
     bufferProvizoriu.Clear;
     bufferProvizoriu.LoadFromFile(OpenFileDialog.FileName);
     ExtensieFile:=ExtractFileExt(OpenFileDialog.Filename);

     if (ExtensieFile='.GEN') or (ExtensieFile='.gen') then begin
                for ContorRand:=0 to bufferProvizoriu.Count-1 do begin
                    Rand := bufferProvizoriu.Strings[ContorRand];
                    if Rand[7] <> ':' then begin FormatCorect:=False;Break;end;end;
                for ContorRand:=0 to bufferProvizoriu.Count-1 do begin
                    Rand:=bufferProvizoriu.Strings[ContorRand];
                    AdresaEronata := 1048576*(CharToInt(Rand[1]))+65536*(CharToInt(Rand[2]))+4096*(CharToInt(Rand[3]))+
                                     256*(CharToInt(Rand[4]))+16*(CharToInt(Rand[5]))+(CharToInt(Rand[6]));
                if AdresaEronata >= MaxLengthFlash then begin AdresaCorecta:=False;Break;end;end;

                if (FormatCorect and AdresaCorecta) then begin
                B_WriteFlash.Enabled := True;
                B_VerifyFlash.Enabled := True;
                //E_FileFlash.Text := NumeFisierFlash;
                //L_FileFlash.Color
                //L_FileFlash.Caption := NumeFisierFlash;
                bufferProgramflash.Clear;
                bufferProgramFlash.Assign(bufferProvizoriu);
                L_NrOcteti_FileFlash.Caption:='Fisierul Contine :'+IntToStr(2*bufferProgramFlash.Count)+' Octeti';
                L_NrOcteti_FileFlash.Visible:=True;
                end
                else if not FormatCorect then
                MessageBox(0,'Format incorect de fisier!','Open File', MB_OK or MB_ICONERROR)
                else if (FormatCorect and not AdresaCorecta) then
                MessageBox(0,PChar('Adresa '+IntToHex(AdresaEronata,4)+'H nu apartine memoriei Flash'+#13#10+
                             'a procesorului ATMEL !'),'Open File', MB_OK or MB_ICONERROR);
     end
     else if (ExtensieFile='.HEX') or (ExtensieFile='.hex') or (ExtensieFile='.A90') or (ExtensieFile='.a90') then begin
                if not (bufferProvizoriu[bufferProvizoriu.Count-1] = ':00000001FF') then FormatCorect:=False
                else FormatCorect:=True;
                    for ContorRand:=0 to bufferProvizoriu.Count-1 do begin
                        Rand:=bufferProvizoriu.Strings[ContorRand];
                        if (Rand[8]+Rand[9]) = '02' then AdresaSegment:= 65536*(CharToInt(Rand[10]))+4096*(CharToInt(Rand[11]))+256*(CharToInt(Rand[12]))+16*(CharToInt(Rand[13]));
                        if (Rand[8]+Rand[9]) = '00' then AdresaEronata := AdresaSegment+16*(CharToInt(Rand[2]))+CharToInt(Rand[3])+4096*(CharToInt(Rand[4]))+256*(CharToInt(Rand[5]))+16*(CharToInt(Rand[6]))+(CharToInt(Rand[7]))-1;
                        if AdresaEronata >= 2*MaxLengthFlash then begin AdresaCorecta:=False;Break;end;
                    end;
                    for ContorRand:=0 to bufferProvizoriu.Count-1 do begin
                        Rand:=bufferProvizoriu.Strings[ContorRand];
                        ByteCount:=16*(CharToInt(Rand[2]))+CharToInt(Rand[3]);
                        tempBuff1:=0;
                        for ContorCampRand:=1 to ByteCount+5 do begin
                                tempBuff0:=(CharToInt(Rand[2*ContorCampRand]) shl 4)+CharToInt(Rand[2*ContorCampRand+1]);
                                tempBuff1:=tempBuff1+tempBuff0;
                        end;
                        if (tempBuff1<>0) then begin CRCCorect:=False;Break;end;end;

                    if (FormatCorect and AdresaCorecta and CRCCorect) then begin
                    B_WriteFlash.Enabled := True;
                    B_VerifyFlash.Enabled := True;
                    E_FileFlash.Text := NumeFisierFlash;
                    //L_FileFlash.Caption := NumeFisierFlash;
                    bufferProgramFlash.Clear;
                    //bufferProvizoriu.SaveToFile('D:\temp1.txt');
                    transfHexInGen;
                    L_NrOcteti_FileFlash.Caption:='Fisierul Contine:  '+IntToStr(2*bufferProgramFlash.Count)+'  Octeti';
                    L_NrOcteti_FileFlash.Visible:=True;
                    end
                    else if not FormatCorect then
                    MessageBox(0,'Format incorect de fisier!','Open File', MB_OK or MB_ICONERROR)
                    else if (FormatCorect and not AdresaCorecta) then
                    MessageBox(0,PChar('Adresa '+IntToHex(AdresaEronata,4)+'H nu apartine memoriei Flash'+#13#10+
                             'a procesorului ATMEL !'),'Open File', MB_OK or MB_ICONERROR)
                    else if (FormatCorect and not CRCCorect) then
                     MessageBox(0,PChar('Eroare de CRC '+#13#10+
                             'Rand  = '+IntToStr(ContorRand+1)),'Open File', MB_OK or MB_ICONERROR);
        end;
end;
end;

        {procedura de translatare a fisierului HEX in GEN}

procedure transfHexInGen;
var
        I,J,ByteCount,Adresa, AdresaSegment: Integer;
        Rand, Camp: String;
begin
AdresaSegment:=0;
for I:=0 to (bufferProvizoriu.Count-2) do begin
                Rand:=bufferProvizoriu[I];
                if (Rand[8]+Rand[9]) = '02' then AdresaSegment:= 65536*(CharToInt(Rand[10]))+4096*(CharToInt(Rand[11]))+
                                                             256*(CharToInt(Rand[12]))+16*(CharToInt(Rand[13]));
                if (Rand[8]+Rand[9]= '00') then begin
                    ByteCount:=16*(CharToInt(Rand[2]))+CharToInt(Rand[3]);
                    Adresa:=trunc((AdresaSegment+ 4096*CharToInt(Rand[4])+256*CharToInt(Rand[5])+16*CharToInt(Rand[6])+CharToInt(Rand[7]))/2);
                    //Camp:='00'+Rand[4]+Rand[5]+Rand[6]+Rand[7]+':'+Rand[10]+Rand[11]+Rand[12]+Rand[13];
                    Camp:=IntToHex(Adresa,6)+':'+Rand[12]+Rand[13]+Rand[10]+Rand[11];
                    bufferProgramFlash.Add(Camp);
                    for J:=1 to trunc((ByteCount+1)/2)-1 do begin
                        Inc(Adresa);
                        Camp:='00'+IntToHex(Adresa,4)+':'+Rand[4*J+12]+Rand[4*J+13]+Rand[4*J+10]+Rand[4*J+11];
                        bufferProgramFlash.Add(Camp);
                    end;
                end;
end;
//BufferProgramFlash.SaveToFile('C:\temp.txt');
end;

    { Trateaza buton OpenFileEE }

procedure TMainForm.B_OpenFileEEPROMClick(Sender: TObject);
var
   Rand: String;
   NumeFisierEE: AnsiString;
   ContorRand, AdresaEronata: Integer;
   EroareFormat, EroareAdresa: Boolean;

begin
EroareFormat:=False;
EroareAdresa:=False;
if OpenFileDialogEE.Execute then begin
     OpenFileDialog.InitialDir := ExtractFilePath(OpenFileDialogEE.FileName);
     NumeFisierEE := ExtractFilePath(OpenFileDialogEE.FileName)+ExtractFileName(OpenFileDialogEE.Filename);
     bufferProvizoriu.Clear;
     bufferProvizoriu.LoadFromFile(OpenFileDialogEE.FileName);
     for ContorRand:=0 to bufferProvizoriu.Count-1 do begin
          Rand := bufferProvizoriu.Strings[ContorRand];
          if Rand[5] <> ':' then begin EroareFormat:=True;Break;end;end;
     for ContorRand:=0 to bufferProvizoriu.Count-1 do begin
         Rand:=bufferProvizoriu.Strings[ContorRand];
         AdresaEronata:=16*(CharToInt(Rand[3]))+(CharToInt(Rand[4]));
         if AdresaEronata >= MaxLengthEE then begin EroareAdresa:=False;Break;end;end;

     if not (EroareFormat and EroareAdresa)then begin
          ReadFileEE:=True;
          B_WriteEEPROM.Enabled := true;
          B_VerifyEEPROM.Enabled := true;
          E_OpenFileEEPROM.Text := NumeFisierEE;
          bufferProgramEE.Clear;
          bufferProgramEE.Assign(bufferProvizoriu);
          L_NrOcteti_FileEE.Caption:='Fisierul contine: '+IntToStr(bufferProgramEE.Count)+' Octeti';
          L_NrOcteti_FileEE.Visible:=True;
          end
     else if EroareFormat then
          MessageBox(0,'Format incorect de Fisier!','Open File', MB_OK or MB_ICONERROR)
     else if (not EroareFormat and EroareAdresa) then
          MessageBox(0,PChar('Adresa '+IntToHex(AdresaEronata,2)+'H nu apartine memoriei EEPROM'+#13#10+
                             'a procesorului ATMEL !'),'Open File', MB_OK or MB_ICONERROR);
end;
end;

    { trateaza buton EraseChip }
    { procedura de scriere pe portul paralel}

procedure ReadWritePORT;
var
   ContorOcteti, ContorBiti: integer;
   ContorIntarziere: Longint;
   OctetOUT, OctetIN, BufferRotitorOUT, BufferRotitorIN: byte;
begin
for ContorOcteti:=1 to 4 do begin
    BufferRotitorOUT := DATA_OUT[ContorOcteti];
    BufferRotitorIN := 0;
    for ContorBiti:=1 to 8 do begin
        OctetIN := ReadPort(PORT_STARE);                 // citeste data din portul de intrare
        OctetIN := OctetIN shr 6;
        OctetIN := OctetIN and 1;
        BufferRotitorIN := BufferRotitorIN or OctetIN;
        if ContorBiti <> 8 then BufferRotitorIN := BufferRotitorIN shl 1;
        OctetOUT := BufferRotitorOUT;
        OctetOUT := OctetOUT and CLK_OFF;
        OctetOUT := OctetOUT and RESET_OFF;
        //OctetOUT := OctetOUT or MascaAlimentare;
        WritePort(PORT_DATE,OctetOUT);         // pune date in portul de iesire
        OctetOUT := OctetOUT or CLK_ON;
        OctetOUT := OctetOUT and RESET_OFF;
        //OctetOUT := OctetOUT or MascaAlimentare;
        for ContorIntarziere:=1 to 3*Intarziere_1us do; // 3 intarziere 3us
        WritePort(PORT_DATE,OctetOUT);         // genereaza front pozitiv CLK
        OctetOUT := OctetOUT and CLK_OFF;
        OctetOUT := OctetOUT and RESET_OFF;
        //OctetOUT := OctetOUT or MascaAlimentare;
        for ContorIntarziere:=1 to 5*Intarziere_1us do; // 5 intarziere 5us
        WritePort(PORT_DATE,OctetOUT);        // genereaza front negativ CLK
        for ContorIntarziere:=1 to 2*Intarziere_1us do; // 2 intarziere 2us
        BufferRotitorOUT := BufferRotitorOUT shl 1;end;
	DATA_IN[ContorOcteti] := BufferRotitorIN;end;
end;

    { procedura de incarcarea a memoriei Flash }

procedure TMainForm.B_WriteFlashClick(Sender: TObject);
begin
     PB_Write.Max:=bufferProgramFlash.Count;
     B_OpenFileFlash.Enabled:=False;
     B_WriteFlash.Enabled:=False;
     B_VerifyFlash.Enabled:=False;
     B_ReadFlash.Enabled:=False;
     B_WriteEEPROMFromList.Enabled:=False;
     B_ReadEEPROM.Enabled:=False;
     B_WriteEEPROM.Enabled:=False;
     B_VerifyEEPROM.Enabled:=False;
     B_ReadEEPROM.Enabled:=False;
     //StatusBar1.Panels[0].Text:='WriteFlash';


if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATtiny2313') then begin
            Write_ATmega168:=TWrite_ATmega168.Create(False);
            Write_ATmega168.OnTerminate:=TerminaProces;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATmega168') then begin
            Write_ATmega168:=TWrite_ATmega168.Create(False);
            Write_ATmega168.OnTerminate:=TerminaProces;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='AT90USB1287') then begin
            Write_AT90USB:=TWrite_AT90USB.Create(False);
            Write_AT90USB.OnTerminate:=TerminaProces;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATmega256RZAV') then begin
            Write_AT90USB:=TWrite_AT90USB.Create(False);
            Write_AT90USB.OnTerminate:=TerminaProces;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='AT90PWM3') then begin
            Write_AT90PWM3:=TWrite_AT90PWM3.Create(False);
            Write_AT90PWM3.OnTerminate:=TerminaProces;
            end
end;

procedure TmainForm.TerminaProces(Sender: TObject);
begin
B_OpenFileFlash.Enabled:=True;
B_WriteFlash.Enabled:=True;
B_VerifyFlash.Enabled:=True;
B_ReadFlash.Enabled:=true;
B_WriteEEPROMFromList.Enabled:=True;
B_ReadEEPROM.Enabled:=True;
if ReadFileEE then begin
   B_WriteEEPROM.Enabled:=True;
   B_VerifyEEPROM.Enabled:=True;end;
B_ReadEEPROM.Enabled:=True;

if not WriteSauNu then
        MessageBox(0,PChar('S-a produs o eroare de scriere la adresa '+IntToHex(EroareAdresa,4)+'H !'),'Download Flash', MB_OK or MB_ICONERROR)
else begin
        MessageBox(0,'Scrierea s-a realizat corect!','Download Flash', MB_OK);
        PB_Write.Position:=0;
end;
end;
    { procedura de scriere a memoriei EEPROM }

procedure TMainForm.B_WriteEEPROMClick(Sender: TObject);
var
   Rand: string;
   ContorRanduri, ContorIntarziere, EroareAdresa: Integer;
   ADRESA: Word;
   DATE: Byte;
   Eroare:Boolean;
begin
Eroare:=False;
PB_Write.Max:=bufferProgramEE.Count-1;
PB_Write.Position:=0;
for ContorRanduri:=0 to bufferProgramEE.Count-1 do begin
    Rand := bufferProgramEE.Strings[ContorRanduri];
    ADRESA := (CharToInt(Rand[1]) shl 12) or (CharToInt(Rand[2]) shl 8) or (CharToInt(Rand[3]) shl 4) or CharToInt(Rand[4]);
    DATE := (CharToInt(Rand[6]) shl 4) or CharToInt(Rand[7]);
    //L_NrOcteti_FileEE.Caption:=IntToHex(ADRESA,4)+':'+IntToHex(DATE,2);

          DATA_OUT[1] := WRITE_EEPROM;
          DATA_OUT[2] := (ADRESA shr 8);
          DATA_OUT[3] := (ADRESA and $00FF);
          DATA_OUT[4] := DATE;
          ReadWritePORT;                                          // scriere
          for ContorIntarziere:=1 to 25000*Intarziere_1us do;     // intarziere la scriere 25ms
          DATA_OUT[1] := READ_EEPROM;
          ReadWritePORT;                                          // citire
          if DATA_OUT[4] <> DATA_IN[4] then begin Eroare:=True;end;
          PB_Write.Position:=PB_Write.Position+1;
end;
if Eroare then begin
    EroareAdresa := 16*(ADRESA shr 4)+(ADRESA and 15);
    MessageBox(0,PChar('S-a produs o eroare de scriere in memoria EEPROM'+#13#10+
                       'la adresa '+IntToHex(EroareAdresa,2)+'H !'),'Write EEPROM', MB_OK or MB_ICONERROR);
end
else begin
    MessageBox(0,PChar('Scrierea s-a realizat corect!'),'Write EEPROM', MB_OK);
    PB_Write.Position:=0;
end;
end;

    { procedura de verificare a datelor din memoria Flash cu datele din Fisier }

procedure TMainForm.B_VerifyFlashClick(Sender: TObject);
begin
        MainForm.PB_Write.Max:=bufferProgramFlash.Count-1;
        VerifyFlash_ATMEL:=TVerifyFlash_ATMEL.Create(False);
        //VerifyFlash_ATMEL.OnTerminate:=TerminaProcesVerify
end;

    { procedura de verificare a datelor din memoria EEPROM cu datele din Fisiser }

procedure TMainForm.B_VerifyEEPROMClick(Sender: TObject);
var
   ContorRanduri, EroareAdresa: Integer;
   CorectSauNu: Bool;
   Rand: string;
   ADRESA: Word;
   DATE: Byte;
begin
CorectSauNu:=True;
for ContorRanduri:=0 to bufferProgramEE.Count-1 do begin
    Rand := bufferProgramEE.Strings[ContorRanduri];
    ADRESA := (CharToInt(Rand[3]) shl 12) or (CharToInt(Rand[4]) shl 8) or (CharToInt(Rand[5]) shl 4) or CharToInt(Rand[6]);
    DATE := (CharToInt(Rand[8]) shl 4) or CharToInt(Rand[9]);

    DATA_OUT[1] := READ_EEPROM;
    DATA_OUT[2] := (ADRESA shr 8);
    DATA_OUT[3] := (ADRESA and $00FF);
    DATA_OUT[4] := 255;
    ReadWritePORT;			                                  // citire
    if DATE <> DATA_IN[4] then begin CorectSauNu := False;Break;end;end;

if not CorectSauNu then begin
    EroareAdresa := 16*(ADRESA shr 4)+(ADRESA and 15);
    MessageBox(0,PChar('Incompatibilitate la adresa '+IntToHex(EroareAdresa,2)+'H'+#13#10+
                       'intre programul din EEPROM si Fisier !'),'Verify EEPROM', MB_OK or MB_ICONERROR);
end
else
    MessageBox(0,'Programul din memoria EEPROM este compatibil cu cel din Fisier !','Verify EEPROM', MB_OK);
end;

    { procedura de incarcare a datelor din Memoria Flash in obiectul memo }

procedure TMainForm.B_ReadFlashClick(Sender: TObject);
begin
 PB_Write.Max:= Adresa_Stop_Flash - Adresa_Start_Flash;
 ReadFlash_ATMEL:=TReadFlash_ATMEL.Create(False);
 //ReadFlash_ATMEL.OnTerminate:=TerminaProces;
end;

    { procedura de incarcare a datelor din memoria EEPROM in editorul memoriei EEPROM }

procedure TMainForm.B_ReadEEPROMClick(Sender: TObject);
var
   Celula, Cod_ASCII: String;
   J,K: Integer ;
   Pagina: Word;
   Adresa: Byte;
   Eroare: Bool;
begin
Eroare:=False;
Cod_ASCII:='';
if (LV_EE.Items.Count<>0) then LV_EE.Items.Clear;
J:=0;
K:=0;
for Pagina:=0 to ((MaxLengthEE shr 4)-1)do begin
    LV_EE.Items.Add;
    LV_EE.Items[J].Caption:=IntToHex((Pagina shl 4),4);
    for Adresa:=0 to 15 do begin
        DATA_OUT[1] := READ_EEPROM;
        DATA_OUT[2] := (Pagina shr 4);
        DATA_OUT[3] := ((Pagina shl 4) + Adresa) and $00FF;
        DATA_OUT[4] := 255;
        ReadWritePORT;
        //if DATA_IN[2] <> DATA_OUT[1] then begin Eroare:=True;Break;end;
        Celula := IntToHex(DATA_IN[4],2);
        Cod_ASCII:=Cod_ASCII+Char(DATA_IN[4]);
        if (Adresa=8) then begin
            Inc(J);
            K:=0;
            LV_EE.Items.Add;
            LV_EE.Items[J].Caption:=IntToHex(((Pagina shl 4) or $08),4);
            LV_EE.Items[J].SubItems.Add('');
            LV_EE.Items[J].SubItems.Strings[K]:=Celula;
        end
        else begin
            LV_EE.Items[J].SubItems.Add('');
            LV_EE.Items[J].SubItems.Strings[K]:=Celula;
            
        end;
        Inc(K);
        if (Adresa=7) then begin
            LV_EE.Items[J].SubItems.Add('');
            LV_EE.Items[J].SubItems.Strings[K]:=Cod_ASCII;
            Cod_ASCII:='';
        end;
        if (Adresa=15)then begin
            LV_EE.Items[J].SubItems.Add('');
            LV_EE.Items[J].SubItems.Strings[K]:=Cod_ASCII;
            Cod_ASCII:='';
            Inc(J);
            K:=0;
        end;
    end;
end;
if Eroare then MessageBox(0,PChar('Eroare de citire din EEPROM!'+#13#10+'La adresa ['+IntToHex(Pagina,4)+']'+#13#10+
                    'Probabil dispozitiv deconectat'),'Read EEPROM', MB_OK or MB_ICONERROR);

end;

    { Inscrierea bitilor de blocare }

function CharToInt(Caracter: Char): Byte;
begin
case caracter of
     '0': CharToInt := 0;
     '1': CharToInt := 1;
     '2': CharToInt := 2;
     '3': CharToInt := 3;
     '4': CharToInt := 4;
     '5': CharToInt := 5;
     '6': CharToInt := 6;
     '7': CharToInt := 7;
     '8': CharToInt := 8;
     '9': CharToInt := 9;
     'a','A': CharToInt := 10;
     'b','B': CharToInt := 11;
     'c','C': CharToInt := 12;
     'd','D': CharToInt := 13;
     'e','E': CharToInt := 14;
     'f','F': CharToInt := 15;
else
    CharToInt := 255;
end;
end;

    { incarca datele din Editorul memoriei EEPROM in memoria EEPROM }

procedure TMainForm.B_WriteEEPROMFromListClick(Sender: TObject);
var
   Adresa, ContorIntarziere: Integer;
   WriteSauNu, ModificatSauNu: Bool;
begin
{
WriteSauNu:=True;
ModificatSauNu:=False;
for Adresa:=0 to MaxLengthEE-1 do begin
    if ModificatEE[Adresa] then begin
     	  ModificatSauNu := True;
          DATA_OUT[1] := WRITE_EEPROM;
          DATA_OUT[2] := 255;
          DATA_OUT[3] := Adresa;
          DATA_OUT[4] := bufferEE[Adresa];
          ReadWritePORT;                                          // scriere
          for ContorIntarziere:=1 to 25000*Intarziere_1us do;     // intarziere la scriere 25ms
          DATA_OUT[1] := READ_EEPROM;
          ReadWritePORT;                                          // citire
          if DATA_OUT[4] <> DATA_IN[4] then begin
             WriteSauNu := False;
             Break;end;
          bufferEEOld[Adresa]:=bufferEE[Adresa];end;end;

if not ModificatSauNu then
   MessageBox(0,'Nu s-a modificat nici un octet in'+#13#10+
                'Editorul memoriei EEPROM !','Download EEPROM', MB_OK or MB_ICONINFORMATION)
else if not WriteSauNu then
   MessageBox(0,PChar('S-a produs o eroare de scriere in memoria EEPROM la adresa '+IntToHex(Adresa,2)+'H !'),'Download EEPROM', MB_OK or MB_ICONERROR)
   else begin
   MessageBox(0,'Scrierea memoriei EEPROM s-a realizat corect !','Download EEPROM', MB_OK);
   end;
   }
end;


procedure TMainForm.S_MemorieFlashReadMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
if (S_MemorieFlashRead.Cursor=crSizeNS) then SSize:=True
else if (S_MemorieFlashRead.Cursor=crDrag) then SDragging := True;
end;

procedure TMainForm.S_MemorieFlashReadMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
begin

if ((Y>=S_MemorieFlashRead.Height-2) or (Y<=2)) then S_MemorieFlashRead.Cursor:=crSizeNS
else S_MemorieFlashRead.Cursor:=crDrag;

if SSize then begin
    if (Y >= S_MemorieFlashRead.Height+16) then begin
        if ((S_MemorieFlashRead.Top + S_MemorieFlashRead.Height)< (S_MemorieFlash.Top + S_MemorieFlash.Height)) then begin
                S_MemorieFlashRead.Height:=S_MemorieFlashRead.Height+16;
                Adresa_Start_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top)/16)*DefaultReadFlash;
                Adresa_Stop_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top+S_MemorieFlashRead.Height)/16)*DefaultReadFlash;
                L_AdresaStart.Caption:=IntToHex(Adresa_Start_Flash,5)+'H';
                L_AdresaStop.Caption:=IntToHex(Adresa_Stop_Flash-1,5)+'H';
                L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash - Adresa_Start_Flash)+' Words';
                L_AdresaStart.Top := S_MemorieFlashRead.Top-12;
                L_AdresaStop.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height+4;
                L_NoFlashRead.Top:= S_MemorieFlashRead.Top + trunc(S_MemorieFlashRead.Height/2)-4;

        end;
    end;
    if (Y <= S_MemorieFlashRead.Height-16) then begin
        if (S_MemorieFlashRead.Height>16) then begin
                S_MemorieFlashRead.Height:=S_MemorieFlashRead.Height-16;
                Adresa_Start_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top)/16)*DefaultReadFlash;
                Adresa_Stop_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top+S_MemorieFlashRead.Height)/16)*DefaultReadFlash;
                L_AdresaStart.Caption:=IntToHex(Adresa_Start_Flash,5)+'H';
                L_AdresaStop.Caption:=IntToHex(Adresa_Stop_Flash-1,5)+'H';
                L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash - Adresa_Start_Flash)+' Words';
                L_AdresaStart.Top := S_MemorieFlashRead.Top-12;
                L_AdresaStop.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height+4;
                L_NoFlashRead.Top:= S_MemorieFlashRead.Top + trunc(S_MemorieFlashRead.Height/2)-4;

        end;
    end;
end;

if SDragging then begin
  if (Y >= S_MemorieFlashRead.Height) then begin
         if (S_MemorieFlashRead.Top<S_MemorieFlash.Top+S_MemorieFlash.Height-S_MemorieFlashRead.Height) then begin
            S_MemorieFlashRead.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height;
            if ((S_MemorieFlashRead.Top + S_MemorieFlashRead.Height)>(S_MemorieFlash.Top + S_MemorieFlash.Height)) then
                S_MemorieFlashRead.Height := S_MemorieFlash.Top + S_MemorieFlash.Height - S_MemorieFlashRead.Top;
            Adresa_Start_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top)/16)*DefaultReadFlash;
            Adresa_Stop_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top+S_MemorieFlashRead.Height)/16)*DefaultReadFlash;
            L_AdresaStart.Caption:=IntToHex(Adresa_Start_Flash,5)+'H';
            L_AdresaStop.Caption:=IntToHex(Adresa_Stop_Flash-1,5)+'H';
            L_AdresaStart.Top := S_MemorieFlashRead.Top-12;
            L_AdresaStop.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height+4;
            L_NoFlashRead.Top:= S_MemorieFlashRead.Top + trunc(S_MemorieFlashRead.Height/2)-4;
            L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash - Adresa_Start_Flash)+' Words';
         end;
  end
  else if (Y <= -S_MemorieFlashRead.Height) then begin
         if (S_MemorieFlashRead.Top>S_MemorieFlash.Top) then begin
            S_MemorieFlashRead.Top := S_MemorieFlashRead.Top - S_MemorieFlashRead.Height;
            if  (S_MemorieFlashRead.Top < S_MemorieFlash.Top) then
                 S_MemorieFlashRead.Top := S_MemorieFlash.Top;
            Adresa_Start_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top)/S_MemorieFlashRead.Height)*DefaultReadFlash;
            Adresa_Stop_Flash := trunc((S_MemorieFlashRead.Top-S_MemorieFlash.Top+S_MemorieFlashRead.Height)/S_MemorieFlashRead.Height)*DefaultReadFlash;
            L_AdresaStart.Caption:=IntToHex(Adresa_Start_Flash,5)+'H';
            L_AdresaStop.Caption:=IntToHex(Adresa_Stop_Flash-1,5)+'H';
            L_AdresaStart.Top := S_MemorieFlashRead.Top-12;
            L_AdresaStop.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height+4;
            L_NoFlashRead.Top:= S_MemorieFlashRead.Top + trunc(S_MemorieFlashRead.Height/2)-4;
            L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash - Adresa_Start_Flash)+' Words';
         end;
  end;
end;
end;

procedure TMainForm.S_MemorieFlashReadMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
SDragging := False;
SSize := False;
S_MemorieFlashRead.Cursor:=crDefault;
end;


procedure TMainForm.OpenSPIClick(Sender: TObject);
var
   ContorIntarziere: Longint;
   OctetOUT: Byte;
begin
     OctetOUT := RESET_OFF;
     OctetOUT := OctetOUT and CLK_OFF;
     //OctetOUT := OctetOUT or MascaAlimentare;
     WritePort(PORT_DATE,OctetOUT);                       // Reset device
     for ContorIntarziere:=1 to 1000*Intarziere_1us do;     // intarziere 100us
     OctetOUT := OctetOUT or RESET_ON;
     //OctetOUT := OctetOUT or MascaAlimentare;
     WritePort(PORT_DATE,OctetOUT);
     for ContorIntarziere:=1 to 1000*Intarziere_1us do;     // intarziere 100us
     OctetOUT := OctetOUT and RESET_OFF;
     //OctetOUT := OctetOUT or MascaAlimentare;
     WritePort(PORT_DATE,OctetOUT);
     for ContorIntarziere:=1 to 100000*Intarziere_1us do;   // intarziere 100ms
     DATA_OUT[1] := SPI_ENABLE_BYTE_1;
     DATA_OUT[2] := SPI_ENABLE_BYTE_2;
     DATA_OUT[3] := 255;
     DATA_OUT[4] := 255;
     ReadWritePORT;
     if DATA_IN[3] = 83 then begin
	            MessageBox(0,'Programarea Seriala a fost acceptata !','Enable SPI', MB_OK);
                MMenu.Items[1].Enabled := True;
                EroareOpenSPI:=False;
                B_ReadFlash.Enabled:=True;
                B_ReadEEPROM.Enabled:=True;
                end                     // Chip Erase = True
     else MessageBox(0,'Programarea Seriala nu a fost acceptata !','Enable SPI', MB_ICONERROR or MB_OK);
end;

    { Tratare buton Ruleaza }

procedure TMainForm.RuleazaClick(Sender: TObject);
var
   OctetOUT: byte;
begin
     OctetOUT := 255;
     WritePort(PORT_DATE,OctetOUT);
     MessageBox(0,'Dispozitiv in rulare !','Run', MB_OK or MB_ICONINFORMATION);
     //OpenSPIClick(Sender);

end;

procedure TMainForm.ChipEraseClick(Sender: TObject);
var
   ContorIntarziere: Integer;
   Adresa: Word;
   EraseSauNu, OpenSauNu: Bool;
begin
EraseSauNu:=True;
OpenSauNu:=True;
DATA_OUT[1] := CHIP_ERASE_BYTE_1;
DATA_OUT[2] := CHIP_ERASE_BYTE_2;
DATA_OUT[3] := 255;
DATA_OUT[4] := 255;
ReadWritePORT;
MessageBox(0,'Memoria a fost stearsa !'+#13#10+'ATENTIE! Memoria nu a fost verificata','Chip Erase', MB_OK);
Contor_EraseWrite:=Contor_EraseWrite+1;
L_Cicluri_EraseWrite.Caption:='Cicluri Erase/Write:  '+IntToStr(Contor_EraseWrite);
end;


procedure TMainForm.TabSetChange(Sender: TObject; NewTab: Integer;
  var AllowChange: Boolean);
begin
    Notebook.PageIndex:=NewTab;
end;


procedure TMainForm.LV_EEEditing(Sender: TObject; Item: TListItem;
  var AllowEdit: Boolean);
begin
AllowEdit:=True;
//L_NrOcteti_FileEE.Caption:=Item.Caption;
//L_NrOcteti_FileEE.Visible:=True;
end;

procedure TMainForm.LV_EEChanging(Sender: TObject; Item: TListItem;
  Change: TItemChange; var AllowChange: Boolean);
begin
AllowChange:=True;
//L_NrOcteti_FileEE.Caption:=Item.Caption;
//L_NrOcteti_FileEE.Visible:=True;
end;

procedure TMainForm.LV_EECustomDrawSubItem(Sender: TCustomListView;
  Item: TListItem; SubItem: Integer; State: TCustomDrawState;
  var DefaultDraw: Boolean);
begin
DefaultDraw:=True;
end;

procedure TMainForm.TB_TimeChange(Sender: TObject);
begin
L_Position.Caption:=IntToStr(TB_Time.Position);
ConstantaTimp:=TB_Time.Position;
Intarziere_1us:=ConstantaTimp*Intarziere_1usDefault;
end;

procedure TMainForm.CB_TipATChange(Sender: TObject);
begin
if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATtiny2313') then begin  MaxLengthFlash:=1024;MaxLengthEE:=128;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='AT90USB1287') then begin  MaxLengthFlash:=65536;MaxLengthEE:=8192;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATmega168') then begin  MaxLengthFlash:=8192;MaxLengthEE:=512;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='ATmega256RZAV') then begin  MaxLengthFlash:=131072;MaxLengthEE:=4096;end
else if (CB_TipAT.Items[CB_TipAT.ItemIndex]='AT90PWM3') then begin  MaxLengthFlash:=8192;MaxLengthEE:=512;end;

if (MaxLengthFlash>=32768) then begin
    Adresa_Start_Flash:=0;
    DefaultReadFlash:=4096;
    Adresa_Stop_Flash:=DefaultReadFlash;
    S_MemorieFlashRead.Top:=S_MemorieFlash.Top;
    S_MemorieFlashRead.Height:= trunc(256/trunc(MaxLengthFlash/DefaultReadFlash));
end
else if ((MaxLengthFlash>=2048) and (MaxLengthFlash<=16384)) then begin
    Adresa_Start_Flash:=0;
    DefaultReadFlash:=512;
    Adresa_Stop_Flash:=DefaultReadFlash;
    S_MemorieFlashRead.Top:=S_MemorieFlash.Top;
    S_MemorieFlashRead.Height:= trunc(256/trunc(MaxLengthFlash/DefaultReadFlash));
end
else begin
    Adresa_Start_Flash:=0;
    DefaultReadFlash:=64;
    Adresa_Stop_Flash:=DefaultReadFlash;
    S_MemorieFlashRead.Top:=S_MemorieFlash.Top;
    S_MemorieFlashRead.Height:= trunc(256/trunc(MaxLengthFlash/DefaultReadFlash));
end;
L_AdresaStart.Caption:=IntToHex(Adresa_Start_Flash,5)+'H';
L_AdresaStop.Caption:=IntToHex(Adresa_Stop_Flash,5)+'H';
L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash - Adresa_Start_Flash)+' Words';
L_AdresaStart.Top := S_MemorieFlashRead.Top-12;
L_AdresaStop.Top := S_MemorieFlashRead.Top + S_MemorieFlashRead.Height+4;
L_NoFlashRead.Top:= S_MemorieFlashRead.Top + trunc(S_MemorieFlashRead.Height/2)-4;
L_NoFlashRead.Caption:= IntToStr(Adresa_Stop_Flash-Adresa_Start_Flash)+': Words';
end;

procedure TMainForm.FormClose(Sender: TObject; var Action: TCloseAction);
var
     Content: TStringList;
     PathFile: array[0..MAX_PATH-1] of Char;
     CurentPath: String;
begin
    GetModuleFileName(hInstance, PathFile, SizeOf(PathFile));
    CurentPath:=ExtractFilePath(PathFile);
    Content:=TStringList.Create;
    Content.Add('TipMicrocontroler='+CB_TipAT.Items[CB_TipAT.ItemIndex]);
    Content.Add('ConstantaTimp='+IntToStr(TB_Time.Position));
    Content.Add('Contor_EraseWrite='+IntToStr(Contor_EraseWrite));
    Content.SaveToFile(CurentPath+'Prog.ini');
end;

//************************************************************
//      Procedura de citire Fuse Bits pentru ATtiny2313
//************************************************************
procedure TMainForm.B_ReadFuse_ATtiny2313Click(Sender: TObject);
const
        READ_LockBB_ATtiny2313_1        = $58;
        READ_LockBB_ATtiny2313_2        = 0;

        READ_FuseExtended_ATtiny2313_1  = $50;
        READ_FuseExtended_ATtiny2313_2  = $08;

        READ_FuseHigh_ATtiny2313_1      = $58;
        READ_FuseHigh_ATtiny2313_2      = $08;

        READ_FuseLow_ATtiny2313_1       = $50;
        READ_FuseLow_ATtiny2313_2       = 0;

var
        Config_Bits_ATtiny2313, Byte1, Byte2, Byte3, Byte4: Byte;
begin
Config_Bits_ATtiny2313:=$ff;
case SB_ATtiny2313.Position of
    1:  begin
            Byte1 := READ_LockBB_ATtiny2313_1;
            Byte2 := READ_LockBB_ATtiny2313_2;
            Byte3 := 0;
        end;

    2:  begin
            Byte1 := READ_FuseExtended_ATtiny2313_1;
            Byte2 := READ_FuseExtended_ATtiny2313_2;
            Byte3 := 0;
        end;
    3:  begin
            Byte1 := READ_FuseHigh_ATtiny2313_1;
            Byte2 := READ_FuseHigh_ATtiny2313_2;
            Byte3 := 0;
        end;
    4:  begin
            Byte1 := READ_FuseLow_ATtiny2313_1;
            Byte2 := READ_FuseLow_ATtiny2313_2;
            Byte3 := 0;
        end;

end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := 0;
ReadWritePORT;
case SB_ATtiny2313.Position of
    1:  begin
            if ((DATA_IN[4] and $01)=0) then CB_LOCK1_ATtiny2313.Checked:=True else CB_Lock1_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_LOCK2_ATtiny2313.Checked:=True else CB_Lock2_ATtiny2313.Checked:=False;
        end;
    2:  begin
            if ((DATA_IN[4] and $01)=0) then CB_SELFPRGEN_ATtiny2313.Checked:=True else CB_SELFPRGEN_ATtiny2313.Checked:=False;
        end;
    3:  begin
            if ((DATA_IN[4] and $01)=0) then CB_RSTDISBL_ATtiny2313.Checked:=True else CB_RSTDISBL_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BODLEVEL0_ATtiny2313.Checked:=True else CB_BODLEVEL0_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BODLEVEL1_ATtiny2313.Checked:=True else CB_BODLEVEL1_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_BODLEVEL2_ATtiny2313.Checked:=True else CB_BODLEVEL2_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_WDTON_ATtiny2313.Checked:=True else CB_WDTON_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SPIEN_ATtiny2313.Checked:=True else CB_SPIEN_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_EESAVE_ATtiny2313.Checked:=True else CB_EESAVE_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_DWEN_ATtiny2313.Checked:=True else CB_DWEN_ATtiny2313.Checked:=False;

        end;
    4:  begin
            if ((DATA_IN[4] and $01)=0) then CB_CKSEL0_ATtiny2313.Checked:=True else CB_CKSEL0_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_CKSEL1_ATtiny2313.Checked:=True else CB_CKSEL1_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_CKSEL2_ATtiny2313.Checked:=True else CB_CKSEL2_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_CKSEL3_ATtiny2313.Checked:=True else CB_CKSEL3_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_SUT0_ATtiny2313.Checked:=True else CB_SUT0_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SUT1_ATtiny2313.Checked:=True else CB_SUT1_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_CKOUT_ATtiny2313.Checked:=True else CB_CKOUT_ATtiny2313.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_CKDIV8_ATtiny2313.Checked:=True else CB_CKDIV8_ATtiny2313.Checked:=False;
        end;
end;
end;

//******************************************************************************
//      Procedura de scriere Fuse Bits pentru ATtiny2313
//******************************************************************************
procedure TMainForm.B_WriteFuse_ATtiny2313Click(Sender: TObject);
const
        WRITE_LockBB_ATtiny2313_1 = $AC;
        WRITE_LockBB_ATtiny2313_2 = $E0;

        WRITE_FuseExtended_ATtiny2313_1 = $AC;
        WRITE_FuseExtended_ATtiny2313_2 = $A4;

        WRITE_FuseHigh_ATtiny2313_1 = $AC;
        WRITE_FuseHigh_ATtiny2313_2 = $A8;

        WRITE_FuseLow_ATtiny2313_1 = $AC;
        WRITE_FuseLow_ATtiny2313_2 = $A0;
var
        Config_Bits_ATtiny2313, Byte1, Byte2, Byte3, Byte4: Byte;
begin
Config_Bits_ATtiny2313:=$ff;
case SB_ATtiny2313.Position of
    1:  begin
            if CB_LOCK1_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fe;
            if CB_LOCK2_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fd;
            Byte1 := WRITE_LockBB_ATtiny2313_1;
            Byte2 := WRITE_LockBB_ATtiny2313_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATtiny2313;

        end;
    2:  begin
            if CB_SELFPRGEN_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fe;
            Byte1 := WRITE_FuseExtended_ATtiny2313_1;
            Byte2 := WRITE_FuseExtended_ATtiny2313_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATtiny2313;

        end;
    3:  begin
            if CB_RSTDISBL_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fe;
            if CB_BODLEVEL0_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fd;
            if CB_BODLEVEL1_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fb;
            if CB_BODLEVEL2_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $f7;
            if CB_WDTON_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $ef;
            if CB_SPIEN_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $df;
            if CB_EESAVE_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $bf;
            if CB_DWEN_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $7f;

            Byte1 := WRITE_FuseHigh_ATtiny2313_1;
            Byte2 := WRITE_FuseHigh_ATtiny2313_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATtiny2313;
        end;
    4:  begin
            if CB_CKSEL0_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fe;
            if CB_CKSEL1_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fd;
            if CB_CKSEL2_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $fb;
            if CB_CKSEL3_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $f7;
            if CB_SUT0_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $ef;
            if CB_SUT1_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $df;
            if CB_CKOUT_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $bf;
            if CB_CKDIV8_ATtiny2313.Checked then  Config_Bits_ATtiny2313 := Config_Bits_ATtiny2313 and $7f;
            Byte1 := WRITE_FuseLow_ATtiny2313_1;
            Byte2 := WRITE_FuseLow_ATtiny2313_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATtiny2313;
        end;
end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := Byte4;
ReadWritePORT;
end;

//******************************************************************************
//      Procedura de citire Fuse Bits pentru ATmega88/168
//******************************************************************************
procedure TMainForm.B_ReadFuseATmega168Click(Sender: TObject);
const
        READ_LockBB_ATmega168_1 = $58;
        READ_LockBB_ATmeag168_2 = 0;

        READ_FuseExtended_ATmega168_1 = $50;
        READ_FuseExtended_ATmega168_2 = $08;

        READ_FuseHigh_ATmega168_1 = $58;
        READ_FuseHigh_ATmega168_2 = $08;

        READ_FuseLow_ATmega168_1 = $50;
        READ_FuseLow_ATmega168_2 = 0;
var
        Byte1,Byte2,Byte3: Byte;
begin
case SB_ATmega168.Position of
    1:  begin
            Byte1 := READ_LockBB_ATmega168_1;
            Byte2 := READ_LockBB_ATmeag168_2;
            Byte3 := 0;
        end;
    2:  begin
            Byte1 := READ_FuseExtended_ATmega168_1;
            Byte2 := READ_FuseExtended_ATmega168_2;
            Byte3 := 0;
        end;
    3:  begin
            Byte1 := READ_FuseHigh_ATmega168_1;
            Byte2 := READ_FuseHigh_ATmega168_2;
            Byte3 := 0;
        end;
    4:  begin
            Byte1 := READ_FuseLow_ATmega168_1;
            Byte2 := READ_FuseLow_ATmega168_2;
            Byte3 := 0;
        end;

end;

DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := 0;
ReadWritePORT;

case SB_ATmega168.Position of
    1:  begin
            if ((DATA_IN[4] and $01)=0) then CB_LOCK1_ATmega168.Checked:=True else CB_Lock1_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_LOCK2_ATmega168.Checked:=True else CB_Lock2_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BLB01_ATmega168.Checked:=True else CB_BLB01_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_BLB02_ATmega168.Checked:=True else CB_BLB02_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_BLB11_ATmega168.Checked:=True else CB_BLB11_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_BLB12_ATmega168.Checked:=True else CB_BLB12_ATmega168.Checked:=False;
        end;
    2:  begin
            if ((DATA_IN[4] and $01)=0) then CB_BOOTRST_ATmega168.Checked:=True else CB_BOOTRST_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BOOTSZ0_ATmega168.Checked:=True else CB_BOOTSZ0_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BOOTSZ1_ATmega168.Checked:=True else CB_BOOTSZ1_ATmega168.Checked:=False;
        end;
    3:  begin
            if ((DATA_IN[4] and $01)=0) then CB_BODLEVEL0_ATmega168.Checked:=True else CB_BODLEVEL0_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BODLEVEL1_ATmega168.Checked:=True else CB_BODLEVEL1_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BODLEVEL2_ATmega168.Checked:=True else CB_BODLEVEL2_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_EESAVE_ATmega168.Checked:=True else CB_EESAVE_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_WDTON_ATmega168.Checked:=True else CB_WDTON_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SPIEN_ATmega168.Checked:=True else CB_SPIEN_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_DWEN_ATmega168.Checked:=True else CB_DWEN_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_RSTDISBL_ATmega168.Checked:=True else CB_RSTDISBL_ATmega168.Checked:=False;
        end;
    4:  begin
            if ((DATA_IN[4] and $01)=0) then CB_CKSEL0_ATmega168.Checked:=True else CB_CKSEL0_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_CKSEL1_ATmega168.Checked:=True else CB_CKSEL1_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_CKSEL2_ATmega168.Checked:=True else CB_CKSEL2_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_CKSEL3_ATmega168.Checked:=True else CB_CKSEL3_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_SUT0_ATmega168.Checked:=True else CB_SUT0_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SUT1_ATmega168.Checked:=True else CB_SUT1_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_CKOUT_ATmega168.Checked:=True else CB_CKOUT_ATmega168.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_CKDIV8_ATmega168.Checked:=True else CB_CKDIV8_ATmega168.Checked:=False;
        end;
end;
end;

//******************************************************************************
//      Procedura de scriere Fuse Bits pentru ATmega88/168
//******************************************************************************
procedure TMainForm.B_WriteFuseATmega168Click(Sender: TObject);
const
        WRITE_LockBB_ATmega168_1 = $AC;
        WRITE_LockBB_ATmega168_2 = $E0;

        WRITE_FuseExtended_ATmega168_1 = $AC;
        WRITE_FuseExtended_ATmega168_2 = $A4;

        WRITE_FuseHigh_ATmega168_1 = $AC;
        WRITE_FuseHigh_ATmega168_2 = $A8;

        WRITE_FuseLow_ATmega168_1 = $AC;
        WRITE_FuseLow_ATmega168_2 = $A0;
var
        Config_Bits_ATmega168, Byte1, Byte2, Byte3, Byte4: Byte;
begin
Config_Bits_ATmega168:=$ff;
case SB_ATmega168.Position of
    1:  begin
            if CB_LOCK1_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fe;
            if CB_LOCK2_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fd;
            if CB_BLB01_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fb;
            if CB_BLB02_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $f7;
            if CB_BLB11_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $ef;
            if CB_BLB12_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $df;
            Byte1 := WRITE_LockBB_ATmega168_1;
            Byte2 := WRITE_LockBB_ATmega168_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATmega168;

        end;
    2:  begin
            if CB_BOOTRST_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fe;
            if CB_BOOTSZ0_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fd;
            if CB_BOOTSZ1_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fb;
            Byte1 := WRITE_FuseExtended_ATmega168_1;
            Byte2 := WRITE_FuseExtended_ATmega168_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATmega168;

        end;
    3:  begin
            if CB_BODLEVEL0_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fe;
            if CB_BODLEVEL1_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fd;
            if CB_BODLEVEL2_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fb;
            if CB_EESAVE_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $f7;
            if CB_WDTON_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $ef;
            if CB_SPIEN_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $df;
            if CB_DWEN_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $bf;
            if CB_RSTDISBL_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $7f;
            Byte1 := WRITE_FuseHigh_ATmega168_1;
            Byte2 := WRITE_FuseHigh_ATmega168_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATmega168;
        end;
    4:  begin
            if CB_CKSEL0_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fe;
            if CB_CKSEL1_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fd;
            if CB_CKSEL2_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $fb;
            if CB_CKSEL3_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $f7;
            if CB_SUT0_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $ef;
            if CB_SUT1_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $df;
            if CB_CKOUT_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $bf;
            if CB_CKDIV8_ATmega168.Checked then  Config_Bits_ATmega168 := Config_Bits_ATmega168 and $7f;
            Byte1 := WRITE_FuseLow_ATmega168_1;
            Byte2 := WRITE_FuseLow_ATmega168_2;
            Byte3 := 0;
            Byte4 := Config_Bits_ATmega168;
        end;
end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := Byte4;
ReadWritePORT;
end;


//******************************************************************************
//      Procedura de citire Fuse Bits pentru AT90USB
//******************************************************************************
procedure TMainForm.B_ReadFuseAT90USBClick(Sender: TObject);
const
        READ_LockBB_AT90USB_1 = $58;
        READ_LockBB_AT90USB_2 = $00;

        READ_FuseExtended_AT90USB_1 = $50;
        READ_FuseExtended_AT90USB_2 = $08;

        READ_FuseHigh_AT90USB_1 = $58;
        READ_FuseHigh_AT90USB_2 = $08;

        READ_FuseLow_AT90USB_1 = $50;
        READ_FuseLow_AT90USB_2 = $00;
var
        Byte1,Byte2,Byte3: Byte;
begin
case SB_AT90USB.Position of
    1:  begin
            Byte1 := READ_LockBB_AT90USB_1;
            Byte2 := READ_LockBB_AT90USB_2;
            Byte3 := 0;
        end;
    2:  begin
            Byte1 := READ_FuseExtended_AT90USB_1;
            Byte2 := READ_FuseExtended_AT90USB_2;
            Byte3 := 0;
        end;
    3:  begin
            Byte1 := READ_FuseHigh_AT90USB_1;
            Byte2 := READ_FuseHigh_AT90USB_2;
            Byte3 := 0;
        end;
    4:  begin
            Byte1 := READ_FuseLow_AT90USB_1;
            Byte2 := READ_FuseLow_AT90USB_2;
            Byte3 := 0;
        end;

end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := 0;
ReadWritePORT;
case SB_AT90USB.Position of
    1:  begin
            if ((DATA_IN[4] and $01)=0) then CB_LOCK1_AT90USB.Checked:=True else CB_Lock1_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_LOCK2_AT90USB.Checked:=True else CB_Lock2_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BLB01_AT90USB.Checked:=True else CB_BLB01_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_BLB02_AT90USB.Checked:=True else CB_BLB02_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_BLB11_AT90USB.Checked:=True else CB_BLB11_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_BLB12_AT90USB.Checked:=True else CB_BLB12_AT90USB.Checked:=False;
        end;
    2:  begin
            if ((DATA_IN[4] and $01)=0) then CB_BODLEVEL0_AT90USB.Checked:=True else CB_BODLEVEL0_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BODLEVEL1_AT90USB.Checked:=True else CB_BODLEVEL1_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BODLEVEL2_AT90USB.Checked:=True else CB_BODLEVEL2_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_HWBE_AT90USB.Checked:=True else CB_HWBE_AT90USB.Checked:=False;
        end;
    3:  begin
            if ((DATA_IN[4] and $01)=0) then CB_BOOTRST_AT90USB.Checked:=True else CB_BOOTRST_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BOOTSZ0_AT90USB.Checked:=True else CB_BOOTSZ0_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BOOTSZ1_AT90USB.Checked:=True else CB_BOOTSZ1_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_EESAVE_AT90USB.Checked:=True else CB_EESAVE_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_WDTON_AT90USB.Checked:=True else CB_WDTON_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SPIEN_AT90USB.Checked:=True else CB_SPIEN_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_JTAGEN_AT90USB.Checked:=True else CB_JTAGEN_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_OCDEN_AT90USB.Checked:=True else CB_OCDEN_AT90USB.Checked:=False;
        end;
    4:  begin
            if ((DATA_IN[4] and $01)=0) then CB_CKSEL0_AT90USB.Checked:=True else CB_CKSEL0_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_CKSEL1_AT90USB.Checked:=True else CB_CKSEL1_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_CKSEL2_AT90USB.Checked:=True else CB_CKSEL2_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_CKSEL3_AT90USB.Checked:=True else CB_CKSEL3_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_SUT0_AT90USB.Checked:=True else CB_SUT0_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SUT1_AT90USB.Checked:=True else CB_SUT1_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_CKOUT_AT90USB.Checked:=True else CB_CKOUT_AT90USB.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_CKDIV8_AT90USB.Checked:=True else CB_CKDIV8_AT90USB.Checked:=False;
        end;
end;
end;

//******************************************************************************
//      Procedura de scriere Fuse Bits pentru AT90USB
//******************************************************************************
procedure TMainForm.B_WriteFuseAT90USBClick(Sender: TObject);
const
        WRITE_LockBB_AT90USB_1 = $AC;
        WRITE_LockBB_AT90USB_2 = $E0;

        WRITE_FuseExtended_AT90USB_1 = $AC;
        WRITE_FuseExtended_AT90USB_2 = $A4;

        WRITE_FuseHigh_AT90USB_1 = $AC;
        WRITE_FuseHigh_AT90USB_2 = $A8;

        WRITE_FuseLow_AT90USB_1 = $AC;
        WRITE_FuseLow_AT90USB_2 = $A0;
var
        Config_Bits_AT90USB, Byte1, Byte2, Byte3, Byte4: Byte;
begin
Config_Bits_AT90USB:=$ff;
case SB_AT90USB.Position of
    1:  begin
            if CB_LOCK1_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fe;
            if CB_LOCK2_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fd;
            if CB_BLB01_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fb;
            if CB_BLB02_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $f7;
            if CB_BLB11_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $ef;
            if CB_BLB12_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $df;
            Byte1 := WRITE_LockBB_AT90USB_1;
            Byte2 := WRITE_LockBB_AT90USB_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90USB;
        end;
    2:  begin
            if CB_BODLEVEL0_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fe;
            if CB_BODLEVEL1_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fd;
            if CB_BODLEVEL2_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fb;
            if CB_HWBE_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $f7;
            Byte1 := WRITE_FuseExtended_AT90USB_1;
            Byte2 := WRITE_FuseExtended_AT90USB_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90USB;
        end;
    3:  begin
            if CB_BOOTRST_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fe;
            if CB_BOOTSZ0_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fd;
            if CB_BOOTSZ1_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fb;
            if CB_EESAVE_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $f7;
            if CB_WDTON_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $ef;
            if CB_SPIEN_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $df;
            if CB_JTAGEN_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $bf;
            if CB_OCDEN_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $7f;
            Byte1 := WRITE_FuseHigh_AT90USB_1;
            Byte2 := WRITE_FuseHigh_AT90USB_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90USB;
        end;
    4:  begin
            if CB_CKSEL0_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fe;
            if CB_CKSEL1_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fd;
            if CB_CKSEL2_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $fb;
            if CB_CKSEL3_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $f7;
            if CB_SUT0_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $ef;
            if CB_SUT1_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $df;
            if CB_CKOUT_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $bf;
            if CB_CKDIV8_AT90USB.Checked then  Config_Bits_AT90USB := Config_Bits_AT90USB and $7f;
            Byte1 := WRITE_FuseLow_AT90USB_1;
            Byte2 := WRITE_FuseLow_AT90USB_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90USB;
        end;
end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := Byte4;
ReadWritePORT;
end;

//******************************************************************************
//      Procedura de citire Fuse Bits pentru AT90PWM
//******************************************************************************
procedure TMainForm.B_ReadFuseAT90PWMClick(Sender: TObject);
const
        READ_LockBB_AT90PWM_1 = $58;
        READ_LockBB_AT90PWM_2 = $00;

        READ_FuseExtended_AT90PWM_1 = $50;
        READ_FuseExtended_AT90PWM_2 = $08;

        READ_FuseHigh_AT90PWM_1 = $58;
        READ_FuseHigh_AT90PWM_2 = $08;

        READ_FuseLow_AT90PWM_1 = $50;
        READ_FuseLow_AT90PWM_2 = $00;
var
        Byte1,Byte2,Byte3: Byte;
begin
case SB_AT90PWM.Position of
    1:  begin
            Byte1 := READ_LockBB_AT90PWM_1;
            Byte2 := READ_LockBB_AT90PWM_2;
            Byte3 := 0;
        end;
    2:  begin
            Byte1 := READ_FuseExtended_AT90PWM_1;
            Byte2 := READ_FuseExtended_AT90PWM_2;
            Byte3 := 0;
        end;
    3:  begin
            Byte1 := READ_FuseHigh_AT90PWM_1;
            Byte2 := READ_FuseHigh_AT90PWM_2;
            Byte3 := 0;
        end;
    4:  begin
            Byte1 := READ_FuseLow_AT90PWM_1;
            Byte2 := READ_FuseLow_AT90PWM_2;
            Byte3 := 0;
        end;
end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := 0;
ReadWritePORT;
case SB_AT90PWM.Position of
    1:  begin
            if ((DATA_IN[4] and $01)=0) then CB_LOCK1_AT90PWM.Checked:=True else CB_Lock1_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_LOCK2_AT90PWM.Checked:=True else CB_Lock2_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BLB01_AT90PWM.Checked:=True else CB_BLB01_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_BLB02_AT90PWM.Checked:=True else CB_BLB02_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_BLB11_AT90PWM.Checked:=True else CB_BLB11_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_BLB12_AT90PWM.Checked:=True else CB_BLB12_AT90PWM.Checked:=False;
        end;
    2:  begin
            if ((DATA_IN[4] and $01)=0) then CB_BOOTRST_AT90PWM.Checked:=True else CB_BOOTRST_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BOOTSZ0_AT90PWM.Checked:=True else CB_BOOTSZ0_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BOOTSZ1_AT90PWM.Checked:=True else CB_BOOTSZ1_AT90PWM.Checked:=False;

            if ((DATA_IN[4] and $10)=0) then CB_PSCRV_AT90PWM.Checked:=True else CB_PSCRV_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_PSC0RB_AT90PWM.Checked:=True else CB_PSC0RB_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_PSC1RB_AT90PWM.Checked:=True else CB_PSC1RB_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_PSC2RB_AT90PWM.Checked:=True else CB_PSC2RB_AT90PWM.Checked:=False;
        end;
    3:  begin
            if ((DATA_IN[4] and $01)=0) then CB_BODLEVEL0_AT90PWM.Checked:=True else CB_BODLEVEL0_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_BODLEVEL1_AT90PWM.Checked:=True else CB_BODLEVEL1_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_BODLEVEL2_AT90PWM.Checked:=True else CB_BODLEVEL2_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_EESAVE_AT90PWM.Checked:=True else CB_EESAVE_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_WDTON_AT90PWM.Checked:=True else CB_WDTON_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SPIEN_AT90PWM.Checked:=True else CB_SPIEN_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_DWEN_AT90PWM.Checked:=True else CB_DWEN_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_RSTDISBL_AT90PWM.Checked:=True else CB_RSTDISBL_AT90PWM.Checked:=False;
        end;
    4:  begin
            if ((DATA_IN[4] and $01)=0) then CB_CKSEL0_AT90PWM.Checked:=True else CB_CKSEL0_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $02)=0) then CB_CKSEL1_AT90PWM.Checked:=True else CB_CKSEL1_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $04)=0) then CB_CKSEL2_AT90PWM.Checked:=True else CB_CKSEL2_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $08)=0) then CB_CKSEL3_AT90PWM.Checked:=True else CB_CKSEL3_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $10)=0) then CB_SUT0_AT90PWM.Checked:=True else CB_SUT0_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $20)=0) then CB_SUT1_AT90PWM.Checked:=True else CB_SUT1_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $40)=0) then CB_CKOUT_AT90PWM.Checked:=True else CB_CKOUT_AT90PWM.Checked:=False;
            if ((DATA_IN[4] and $80)=0) then CB_CKDIV8_AT90PWM.Checked:=True else CB_CKDIV8_AT90PWM.Checked:=False;
        end;
end;
end;

//******************************************************************************
//      Procedura de scriere Fuse Bits pentru AT90PWM
//******************************************************************************
procedure TMainForm.B_WriteFuseAT90PWMClick(Sender: TObject);
const
        WRITE_LockBB_AT90PWM_1 = $AC;
        WRITE_LockBB_AT90PWM_2 = $E0;

        WRITE_FuseExtended_AT90PWM_1 = $AC;
        WRITE_FuseExtended_AT90PWM_2 = $A4;

        WRITE_FuseHigh_AT90PWM_1 = $AC;
        WRITE_FuseHigh_AT90PWM_2 = $A8;

        WRITE_FuseLow_AT90PWM_1 = $AC;
        WRITE_FuseLow_AT90PWM_2 = $A0;
var
        Config_Bits_AT90PWM, Byte1, Byte2, Byte3, Byte4: Byte;
begin
Config_Bits_AT90PWM:=$ff;
case SB_AT90PWM.Position of
    1:  begin
            if CB_LOCK1_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fe;
            if CB_LOCK2_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fd;
            if CB_BLB01_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fb;
            if CB_BLB02_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $f7;
            if CB_BLB11_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $ef;
            if CB_BLB12_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $df;
            Byte1 := WRITE_LockBB_AT90PWM_1;
            Byte2 := WRITE_LockBB_AT90PWM_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90PWM;
        end;
    2:  begin
            if CB_BOOTRST_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fe;
            if CB_BOOTSZ0_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fd;
            if CB_BOOTSZ1_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fb;

            if CB_PSCRV_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $ef;
            if CB_PSC0RB_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $df;
            if CB_PSC1RB_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $bf;
            if CB_PSC2RB_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $7f;
            Byte1 := WRITE_FuseExtended_AT90PWM_1;
            Byte2 := WRITE_FuseExtended_AT90PWM_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90PWM;

        end;
    3:  begin
            if CB_BODLEVEL0_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fe;
            if CB_BODLEVEL1_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fd;
            if CB_BODLEVEL2_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fb;
            if CB_EESAVE_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $f7;
            if CB_WDTON_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $ef;
            if CB_SPIEN_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $df;
            if CB_DWEN_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $bf;
            if CB_RSTDISBL_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $7f;
            Byte1 := WRITE_FuseHigh_AT90PWM_1;
            Byte2 := WRITE_FuseHigh_AT90PWM_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90PWM;
        end;
    4:  begin
            if CB_CKSEL0_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fe;
            if CB_CKSEL1_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fd;
            if CB_CKSEL2_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $fb;
            if CB_CKSEL3_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $f7;
            if CB_SUT0_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $ef;
            if CB_SUT1_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $df;
            if CB_CKOUT_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $bf;
            if CB_CKDIV8_AT90PWM.Checked then  Config_Bits_AT90PWM := Config_Bits_AT90PWM and $7f;
            Byte1 := WRITE_FuseLow_AT90PWM_1;
            Byte2 := WRITE_FuseLow_AT90PWM_2;
            Byte3 := 0;
            Byte4 := Config_Bits_AT90PWM;
        end;
end;
DATA_OUT[1] := Byte1;
DATA_OUT[2] := Byte2;
DATA_OUT[3] := Byte3;
DATA_OUT[4] := Byte4;
ReadWritePORT;
end;



//******************************************************************************
//
//      Subrutine scroll bar
//
//******************************************************************************
procedure TMainForm.SB_ATtiny2313Change(Sender: TObject);
begin
case SB_ATtiny2313.Position of

    1:  begin
            MainForm.GB_LockBitByte_ATtiny2313.Left:=10+130;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Left:=10+2*130;
            MainForm.GB_FuseHighByte_ATtiny2313.Left:=10+3*130;
            MainForm.GB_FuseLowByte_ATtiny2313.Left:=10+4*130;
            MainForm.GB_LockBitByte_ATtiny2313.Font.Color:=clRed;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_ATtiny2313.Font.Color:=clBlue;
        end;
    2:  begin
            MainForm.GB_LockBitByte_ATtiny2313.Left:=10;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Left:=10+130;
            MainForm.GB_FuseHighByte_ATtiny2313.Left:=10+2*130;
            MainForm.GB_FuseLowByte_ATtiny2313.Left:=10+3*130;
            MainForm.GB_LockBitByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Font.Color:=clRed;
            MainForm.GB_FuseHighByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_ATtiny2313.Font.Color:=clBlue;
        end;
    3:  begin
            MainForm.GB_LockBitByte_ATtiny2313.Left:=10-130;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Left:=10;
            MainForm.GB_FuseHighByte_ATtiny2313.Left:=10+130;
            MainForm.GB_FuseLowByte_ATtiny2313.Left:=10+2*130;
            MainForm.GB_LockBitByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_ATtiny2313.Font.Color:=clRed;
            MainForm.GB_FuseLowByte_ATtiny2313.Font.Color:=clBlue;
        end;
    4:  begin
            MainForm.GB_LockBitByte_ATtiny2313.Left:=10-2*130;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Left:=10-130;
            MainForm.GB_FuseHighByte_ATtiny2313.Left:=10;
            MainForm.GB_FuseLowByte_ATtiny2313.Left:=10+130;
            MainForm.GB_LockBitByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_ATtiny2313.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_ATtiny2313.Font.Color:=clRed;
        end;
end;

end;

procedure TMainForm.SB_ATmega168Change(Sender: TObject);
begin
case SB_ATmega168.Position of

    1:  begin
            MainForm.GB_LockBitByte_ATmega168.Left:=10+130;
            MainForm.GB_ExtendedFuseByte_ATmega168.Left:=10+2*130;
            MainForm.GB_FuseHighByte_ATmega168.Left:=10+3*130;
            MainForm.GB_FuseLowByte_ATmega168.Left:=10+4*130;
            MainForm.GB_LockBitByte_ATmega168.Font.Color:=clRed;
            MainForm.GB_ExtendedFuseByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_ATmega168.Font.Color:=clBlue;
        end;
    2:  begin
            MainForm.GB_LockBitByte_ATmega168.Left:=10;
            MainForm.GB_ExtendedFuseByte_ATmega168.Left:=10+130;
            MainForm.GB_FuseHighByte_ATmega168.Left:=10+2*130;
            MainForm.GB_FuseLowByte_ATmega168.Left:=10+3*130;
            MainForm.GB_LockBitByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_ATmega168.Font.Color:=clRed;
            MainForm.GB_FuseHighByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_ATmega168.Font.Color:=clBlue;
        end;
    3:  begin
            MainForm.GB_LockBitByte_ATmega168.Left:=10-130;
            MainForm.GB_ExtendedFuseByte_ATmega168.Left:=10;
            MainForm.GB_FuseHighByte_ATmega168.Left:=10+130;
            MainForm.GB_FuseLowByte_ATmega168.Left:=10+2*130;
            MainForm.GB_LockBitByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_ATmega168.Font.Color:=clRed;
            MainForm.GB_FuseLowByte_ATmega168.Font.Color:=clBlue;
        end;
    4:  begin
            MainForm.GB_LockBitByte_ATmega168.Left:=10-2*130;
            MainForm.GB_ExtendedFuseByte_ATmega168.Left:=10-130;
            MainForm.GB_FuseHighByte_ATmega168.Left:=10;
            MainForm.GB_FuseLowByte_ATmega168.Left:=10+130;
            MainForm.GB_LockBitByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_ATmega168.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_ATmega168.Font.Color:=clRed;
        end;
end;
end;
procedure TMainForm.SB_AT90USBChange(Sender: TObject);
begin
case SB_AT90USB.Position of

    1:  begin
            MainForm.GB_LockBitByte_AT90USB.Left:=10+130;
            MainForm.GB_ExtendedFuseByte_AT90USB.Left:=10+2*130;
            MainForm.GB_FuseHighByte_AT90USB.Left:=10+3*130;
            MainForm.GB_FuseLowByte_AT90USB.Left:=10+4*130;
            MainForm.GB_LockBitByte_AT90USB.Font.Color:=clRed;
            MainForm.GB_ExtendedFuseByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_AT90USB.Font.Color:=clBlue;
        end;
    2:  begin
            MainForm.GB_LockBitByte_AT90USB.Left:=10;
            MainForm.GB_ExtendedFuseByte_AT90USB.Left:=10+130;
            MainForm.GB_FuseHighByte_AT90USB.Left:=10+2*130;
            MainForm.GB_FuseLowByte_AT90USB.Left:=10+3*130;
            MainForm.GB_LockBitByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_AT90USB.Font.Color:=clRed;
            MainForm.GB_FuseHighByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_AT90USB.Font.Color:=clBlue;
        end;
    3:  begin
            MainForm.GB_LockBitByte_AT90USB.Left:=10-130;
            MainForm.GB_ExtendedFuseByte_AT90USB.Left:=10;
            MainForm.GB_FuseHighByte_AT90USB.Left:=10+130;
            MainForm.GB_FuseLowByte_AT90USB.Left:=10+2*130;
            MainForm.GB_LockBitByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_AT90USB.Font.Color:=clRed;
            MainForm.GB_FuseLowByte_AT90USB.Font.Color:=clBlue;
        end;
    4:  begin
            MainForm.GB_LockBitByte_AT90USB.Left:=10-2*130;
            MainForm.GB_ExtendedFuseByte_AT90USB.Left:=10-130;
            MainForm.GB_FuseHighByte_AT90USB.Left:=10;
            MainForm.GB_FuseLowByte_AT90USB.Left:=10+130;
            MainForm.GB_LockBitByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_AT90USB.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_AT90USB.Font.Color:=clRed;
        end;
end;
end;



procedure TMainForm.SB_AT90PWMChange(Sender: TObject);
begin
case SB_AT90PWM.Position of

    1:  begin
            MainForm.GB_LockBitByte_AT90PWM.Left:=10+130;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Left:=10+2*130;
            MainForm.GB_FuseHighByte_AT90PWM.Left:=10+3*130;
            MainForm.GB_FuseLowByte_AT90PWM.Left:=10+4*130;
            MainForm.GB_LockBitByte_AT90PWM.Font.Color:=clRed;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_AT90PWM.Font.Color:=clBlue;
        end;
    2:  begin
            MainForm.GB_LockBitByte_AT90PWM.Left:=10;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Left:=10+130;
            MainForm.GB_FuseHighByte_AT90PWM.Left:=10+2*130;
            MainForm.GB_FuseLowByte_AT90PWM.Left:=10+3*130;
            MainForm.GB_LockBitByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Font.Color:=clRed;
            MainForm.GB_FuseHighByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_AT90PWM.Font.Color:=clBlue;
        end;
    3:  begin
            MainForm.GB_LockBitByte_AT90PWM.Left:=10-130;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Left:=10;
            MainForm.GB_FuseHighByte_AT90PWM.Left:=10+130;
            MainForm.GB_FuseLowByte_AT90PWM.Left:=10+2*130;
            MainForm.GB_LockBitByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_AT90PWM.Font.Color:=clRed;
            MainForm.GB_FuseLowByte_AT90PWM.Font.Color:=clBlue;
        end;
    4:  begin
            MainForm.GB_LockBitByte_AT90PWM.Left:=10-2*130;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Left:=10-130;
            MainForm.GB_FuseHighByte_AT90PWM.Left:=10;
            MainForm.GB_FuseLowByte_AT90PWM.Left:=10+130;
            MainForm.GB_LockBitByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_ExtendedFuseByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_FuseHighByte_AT90PWM.Font.Color:=clBlue;
            MainForm.GB_FuseLowByte_AT90PWM.Font.Color:=clRed;
        end;
end;
end;

procedure TMainForm.B_TestClick(Sender: TObject);
begin

Toggle_LED_TEST := LED_RESET_ON;
TimerTest.Enabled := True;
B_Test.Enabled := False;
end;


procedure TMainForm.TimerTestTimer(Sender: TObject);
var
 Bitmap: TBitmap;
begin
Bitmap := TBitmap.Create;
Bitmap.Height := 16;
Bitmap.Width := 16;
Bitmap.Transparent := True;
IL_led.getBitmap(0,Bitmap);
WritePort(PORT_DATE, $00);

  case(Toggle_LED_TEST) of
       LED_RESET_ON:        begin
                            IL_led.getBitmap(0,Bitmap);
                            LED_RESET.Picture.Bitmap := Bitmap;
                            Toggle_LED_TEST := LED_RESET_OFF;
                            WritePort(PORT_DATE, RESET_ON);
                            end;
       LED_RESET_OFF:       begin
                            IL_led.getBitmap(1,Bitmap);
                            LED_RESET.Picture.Bitmap := Bitmap;
                            Toggle_LED_TEST := LED_CLK_ON;
                            WritePort(PORT_DATE, $00);
                            end;
       LED_CLK_ON:          begin
                            IL_led.getBitmap(0,Bitmap);
                            LED_CLK.Picture.Bitmap := Bitmap;
                            Toggle_LED_TEST := LED_CLK_OFF;
                            WritePort(PORT_DATE, CLK_ON);
                            end;
       LED_CLK_OFF:         begin
                            IL_led.getBitmap(1,Bitmap);
                            LED_CLK.Picture.Bitmap := Bitmap;
                            Toggle_LED_TEST := LED_DATA_OUT_ON;
                            WritePort(PORT_DATE, $00);
                            end;
       LED_DATA_OUT_ON:     begin
                            IL_led.getBitmap(0,Bitmap);
                            LED_DATA_OUT.Picture.Bitmap := Bitmap;
                            Toggle_LED_TEST := LED_DATA_OUT_OFF;
                            WritePort(PORT_DATE, DATA_OUT_ON);
                            if ((ReadPort(PORT_STARE) and $40) = $40) then begin
                                IL_led.getBitmap(0,Bitmap);
                                LED_DATA_IN.Picture.Bitmap := Bitmap;
                            end
                            else begin
                                IL_led.getBitmap(1,Bitmap);
                                LED_DATA_IN.Picture.Bitmap := Bitmap;
                            end;
                            end;
       LED_DATA_OUT_OFF:    begin
                            IL_led.getBitmap(1,Bitmap);
                            LED_DATA_OUT.Picture.Bitmap := Bitmap;
                            //WritePort(PORT_DATE, DATA_OUT_OFF);
                            WritePort(PORT_DATE, $00);
                            if ((ReadPort(PORT_STARE) and $40) = $40) then begin
                                IL_led.getBitmap(0,Bitmap);
                                LED_DATA_IN.Picture.Bitmap := Bitmap;
                            end
                            else begin
                                IL_led.getBitmap(1,Bitmap);
                                LED_DATA_IN.Picture.Bitmap := Bitmap;
                            end;
                            //Toggle_LED_TEST := 10;
                            TimerTest.Enabled := False;
                            B_Test.Enabled := True;
                            end;

  end;
end;

end.













