{$mode objfpc}{$H+}
unit generics.defaults;
interface
type
  generic TComparison<T> = function(constref L, R: T): Integer;
  generic TComparer<T> = class
  public
    FCmp: specialize TComparison<T>;
    class function Construct(const C: specialize TComparison<T>): TComparer;
    function Compare(constref L, R: T): Integer;
  end;
implementation
class function TComparer.Construct(const C: specialize TComparison<T>): TComparer;
begin
  Result := TComparer.Create;
  Result.FCmp := C
end;
function TComparer.Compare(constref L, R: T): Integer;
begin
  Result := FCmp(L, R)
end;
end.
