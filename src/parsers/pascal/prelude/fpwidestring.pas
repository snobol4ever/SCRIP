{$mode objfpc}
unit fpwidestring;
interface
var
  DefaultCollationName : UnicodeString;
function SetActiveCollation(const AName : UnicodeString) : Boolean;
implementation
function SetActiveCollation(const AName : UnicodeString) : Boolean;
begin
  Result := False;
end;
end.
