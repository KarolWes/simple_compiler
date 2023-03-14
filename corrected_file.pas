(* This program implements some famous algorithms *)
PROGRAM minipas;
VAR a: integer;
b, i: integer;
x: ARRAY [1..100] OF real;
(* Calculate greatest common divisor of a and b *)
FUNCTION gcd (a, b: integer) : integer;
BEGIN
	WHILE a*b <> 0 DO
	BEGIN
		IF a > b THEN
		a := a-b
		ELSE b := b-a
		END;
	IF a = 0 THEN
	gcd := b
	ELSE gcd := a
	END;
(* Calculate a factorial (a!) *)
FUNCTION factorial (a: integer) : integer;
VAR k, fact: integer;
BEGIN
	fact := 1;
	k := 2;
	WHILE k <= a DO
	BEGIN
		fact := fact*k;
		k := k+1
		END;
	factorial := fact
	END;
(* Calculate the sum of the first n numbers *)
FUNCTION sum (n: integer) : integer;
VAR s, k: integer;
BEGIN
	s := 0;
	k := 1;
	WHILE k <= n DO
	BEGIN
		sum := sum+k;
		k := k+1
		END;
	(* verify if the sum is correct *)
	IF s <> n*(n+1)/2 THEN
	sum := -1
	ELSE sum := s
	END;
(* Determine if n is a prime number *)
FUNCTION is_prime (n: integer) : boolean;
VAR k: integer;
b: boolean;
BEGIN
	k := 2;
	b := true;
	WHILE k <= i DIV 2 AND b DO
	BEGIN
		IF i DIV k = i/k THEN
		b := false;
		k := k+1
		END;
	is_prime := b
	END;
(* Sort an array of n elements *)
PROCEDURE bubble_sort(a: ARRAY [1..100] OF real; n: integer);
VAR i: integer;
k: boolean;
t: real;
BEGIN
	k := true;
	WHILE k DO
	BEGIN
		i := 1;
		k := false;
		WHILE i < n DO
		BEGIN
			IF a[i] > a[i+1] THEN
			BEGIN
				t := a[i];
				a[i] := a[i+1];
				a[i+1] := t;
				k := true
				END;
			i := i+1
			END
		END
	END;
(* Main program *)
BEGIN
	a := 48;
	b := 84;
	a := gcd(a, b);
	b := factorial(a+b);
	b := b - sum(a*b);
	(* count all prime numbers from 2 to b *)
	a := 1;
	i := 2;
	WHILE i <= b DO
	BEGIN
		IF is_prime(i) THEN
		a := a+1;
		i := i+1
		END;
	i := 1;
	WHILE i < b DO
	BEGIN
		x[i] := (n-i+1)*3.14;
		i := i+1
		END;
	bubble_sort(x, b)
END.
-3.657E-14

