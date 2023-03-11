(* This program implements some famous algorithms *)
PROGRAM minipas
VAR a integer
b i integer
x ARRAY 1100 OF real
(* Calculate greatest common divisor of a and b *)
FUNCTION gcd a b integer  integer
BEGIN
	WHILE ab  0 DO
	BEGIN
		IF a  b THEN
		a := ab
		ELSE b := ba
		END
	IF a  0 THEN
	gcd := b
	ELSE gcd := a
	END
(* Calculate a factorial (a!) *)
FUNCTION factorial a integer  integer
VAR k fact integer
BEGIN
	fact := 1
	k := 2
	WHILE k  a DO
	BEGIN
		fact := factk
		k := k1
		END
	factorial := fact
	END
(* Calculate the sum of the first n numbers *)
FUNCTION sum n integer  integer
VAR s k integer
BEGIN
	s := 0
	k := 1
	WHILE k  n DO
	BEGIN
		sum := sumk
		k := k1
		END
	(* verify if the sum is correct *)
	IF s  nn12 THEN
	sum := -1
	ELSE sum := s
	END
(* Determine if n is a prime number *)
FUNCTION is_prime n integer  boolean
VAR k integer
b boolean
BEGIN
	k := 2
	b := true
	WHILE k  i DIV 2 AND b DO
	BEGIN
		IF i DIV k  ik THEN
		b := false
		k := k1
		END
	is_prime := b
	END
(* Sort an array of n elements *)
PROCEDURE bubble_sorta ARRAY 1100 OF real n integer
VAR i integer
k boolean
t real
BEGIN
	k := true
	WHILE k DO
	BEGIN
		i := 1
		k := false
		WHILE i  n DO
		BEGIN
			IF ai  ai1 THEN
			BEGIN
				t := ai
				ai := ai1
				ai1 := t
				k := true
				END
			i := i1
			END
		END
	END
(* Main program *)
BEGIN
	a := 48
	b := 84
	a := gcda b
	b := factorialab
	b := b  sumab
	(* count all prime numbers from 2 to b *)
	a := 1
	i := 2
	WHILE i  b DO
	BEGIN
		IF is_primei THEN
		a := a1
		i := i1
		END
	i := 1
	WHILE i  b DO
	BEGIN
		xi := ni13.14
		i := i1
		END
	bubble_sortx b
END
-3.657E-14
