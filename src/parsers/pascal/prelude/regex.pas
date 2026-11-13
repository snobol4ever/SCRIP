{$mode objfpc}{$H+}
unit regex;
interface
uses RegExpr;
type
  TRegexError = (recNone, recSuddenEnd, recMetaChar, recNoCloseParen, recExtraChars);
  TRegexEngine = class
  private
    FPat: string;
    FRe: TRegExpr;
  public
    constructor Create(const aRegexStr: string);
    function Parse(out aErrorPos: integer; out aErrorCode: TRegexError): boolean;
    function MatchString(const S: string; out MatchPos: integer; var Offset: integer): boolean;
  end;
implementation
constructor TRegexEngine.Create(const aRegexStr: string);
begin
  FPat := aRegexStr;
  FRe := nil
end;
function TRegexEngine.Parse(out aErrorPos: integer; out aErrorCode: TRegexError): boolean;
begin
  aErrorPos := 0;
  aErrorCode := recNone;
  FRe := TRegExpr.Create(FPat);
  Parse := true
end;
function TRegexEngine.MatchString(const S: string; out MatchPos: integer; var Offset: integer): boolean;
var sub: string;
begin
  MatchPos := 0;
  sub := Copy(S, Offset, Length(S));
  MatchString := FRe.Exec(sub);
  if MatchString then MatchPos := Offset + FRe.MatchPos[0] - 1
end;
end.
