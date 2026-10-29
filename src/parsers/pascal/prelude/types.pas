{$H+}
unit types;
interface
type
  TIntegerDynArray = array of Integer;
  TCardinalDynArray = array of Cardinal;
  TInt64DynArray = array of Int64;
  TStringDynArray = array of string;
  TDoubleDynArray = array of Double;
  TSingleDynArray = array of Single;
  TByteDynArray = array of Byte;
  TBooleanDynArray = array of Boolean;
  TPoint = record
    x, y: LongInt;
  end;
  TRect = record
    Left, Top, Right, Bottom: LongInt;
  end;
implementation
end.
