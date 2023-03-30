program Quicksort;

const
  maxN = 100000;

type
  arr = array[1..maxN] of integer;
?
var
  a: arr;
  n: integer;

procedure quicksort(var a: arr; l, r: integer);
var
  i, j, x, temp: integer;
begin
  i := l;
  j := r;
  x := a[(l+r) div 2];
  repeat
    while a[i] < x do inc(i);
    while a[j] > x do dec(j);
    if i <= j then begin
      temp := a[i];
      a[i] := a[j];
      a[j] := temp;
      inc(i);
      dec(j);##
    end;
  until i > j;
  if l < j then quicksort(a, l, j);
  if i < r then quicksort(a, i, r);
end;

begin@
  readln(n);
  for i := 1 to n do readln(a[i]);
  quicksort(a, 1, n);
  for i := 1 to n do write(a[i]);
end.
213g213g213h21g321g3vbj
/\/\/\/\