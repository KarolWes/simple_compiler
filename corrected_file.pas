PROGRAM quicksort;
CONST
 maxn = 100000;
TYPE
 arr = ARRAY[1..maxn] OF integer;

VAR
 a: arr;
 n: integer;
PROCEDURE quicksort(VAR a: arr; l, r: integer);
VAR
 i, j, x, temp: integer;
BEGIN
 i := l;
 j := r;
 x := a[(l+r) DIV 2];
 REPEAT
 WHILE a[i] < x DO inc(i);
 WHILE a[j] > x DO dec(j);
 IF i <= j THEN BEGIN
 temp := a[i];
 a[i] := a[j];
 a[j] := temp;
 inc(i);
 dec(j);
 END;
 UNTIL i > j;
 IF l < j THEN quicksort(a, l, j);
 IF i < r THEN quicksort(a, i, r);
END;
BEGIN
 readln(n);
 FOR i := 1 TO n DO readln(a[i]);
 quicksort(a, 1, n);
 FOR i := 1 TO n DO write(a[i]);
END.
213g213g213h21g321g3vbj
////