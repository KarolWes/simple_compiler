# program: minipas

# program: gcd

result:	.word	0


.section .text
lab WHILE0
fjp END0
fjp ELSE1
j END1
lab ELSE1
lab END1
j WHILE0
lab END0
fjp ELSE2
j END2
lab ELSE2
lab END2
stp
#______________
# program: factorial

.section .data
k:	.word	0
fact:	.word	0
result:	.word	0


.section .text
lab WHILE3
fjp END3
j WHILE3
lab END3
stp
#______________
# program: sum

.section .data
s:	.word	0
k:	.word	0
result:	.word	0


.section .text
lab WHILE4
fjp END4
j WHILE4
lab END4
fjp ELSE5
j END5
lab ELSE5
lab END5
stp
#______________
# program: is_prime

.section .data
k:	.word	0
b:	.word	0
result:	.word	0


.section .text
lab WHILE6
fjp END6
fjp END7
lab END7
j WHILE6
lab END6
stp
#______________
# program: bubble_sort

.section .data
i:	.word	0
k:	.word	0
t:	.word	0.0


.section .text
lab WHILE8
fjp END8
lab WHILE9
fjp END9
fjp END10
lab END10
j WHILE9
lab END9
j WHILE8
lab END8
stp
#______________
.section .data
a:	.word	0
b:	.word	0
i:	.word	0
n:	.word	0
x:	.space	800	# array of floats


.section .text
lab WHILE11
fjp END11
fjp END12
lab END12
j WHILE11
lab END11
lab WHILE13
fjp END13
j WHILE13
lab END13
stp
#______________
