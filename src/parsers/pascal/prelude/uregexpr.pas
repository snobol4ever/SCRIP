{$mode objfpc}{$H+}
unit uregexpr;
interface
uses RegExpr;
function ExecRegExpr(const ARegExpr, AInputStr: string): boolean;
function ReplaceRegExpr(const ARegExpr, AInputStr, AReplaceStr: string; AUseSubstitution: boolean): string;
implementation
function ExecRegExpr(const ARegExpr, AInputStr: string): boolean;
begin
  ExecRegExpr := RegExpr.ExecRegExpr(ARegExpr, AInputStr);
end;
function ReplaceRegExpr(const ARegExpr, AInputStr, AReplaceStr: string; AUseSubstitution: boolean): string;
begin
  ReplaceRegExpr := RegExpr.ReplaceRegExpr(ARegExpr, AInputStr, AReplaceStr, AUseSubstitution);
end;
end.
