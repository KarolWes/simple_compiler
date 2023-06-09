program minipas;

  var a: integer;
      b, i, n: integer;
      x: array [1..100] of real;


begin
  a := 48;
  b := 84;
  { count all prime numbers from 2 to b }
  a := 1;
  i := 2;
  while i <= b do
  begin
    if a < 10 then
      a := a+1;
    i := i+1
  end;

  i := 1;
  while i < b do
  begin
    x[i] := (n-i+1)*3.14;
    i := i+1
  end
end.
