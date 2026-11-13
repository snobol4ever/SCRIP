unit process;
interface
uses sysutils;
type
  EProcess = class(Exception);
  TProcess = class
  private
    FActive: boolean;
    FExecutable: string;
    procedure SetActive(AValue: boolean);
  public
    constructor Create(AOwner: TObject);
    procedure Execute;
    property Active: boolean read FActive write SetActive;
    property Executable: string read FExecutable write FExecutable;
  end;
implementation
constructor TProcess.Create(AOwner: TObject);
begin
  FActive := false;
  FExecutable := ''
end;
procedure TProcess.Execute;
begin
  if FExecutable = '' then raise EProcess.Create('Executable not set');
  FActive := true
end;
procedure TProcess.SetActive(AValue: boolean);
begin
  if AValue then Execute
  else FActive := false
end;
end.
