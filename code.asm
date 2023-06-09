# program: minipas

.section .data
a:	.word	0
b:	.word	0
i:	.word	0
n:	.word	0
x:	.space	800	# array of floats


.section .text
li t0, 48
sw t0, a
li t0, 84
sw t0, b
li t0, 1
sw t0, a
li t0, 2
sw t0, i
lab WHILE0
lw t0, i
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
lw t0, b
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
sle t0 t0, t0, t1
fjp END0
lw t0, a
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
li t0, 10
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
slt t0 t0, t0, t1
fjp END1
lw t0, a
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
li t0, 1
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
add t0, t0, t1
sw t0, a
lab END1
lw t0, i
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
li t0, 1
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
add t0, t0, t1
sw t0, i
j WHILE0
lab END0
li t0, 1
sw t0, i
lab WHILE2
lw t0, i
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
lw t0, b
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
slt t0 t0, t0, t1
fjp END2
lw t0, i
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
lw t0, n
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
lw t0, i
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
li t0, 1
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
add t0, t0, t1
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
sub t0, t0, t1
li f0, 3.140000
lw $f1, 0($sp)	# pop
addi $sp, $sp, 8
mult f0, f0, f1
sw f0, x
lw t0, i
addi $sp, $sp, -4	# push
sw $t0, 0($sp)
li t0, 1
lw $t1, 0($sp)	# pop
addi $sp, $sp, 4
add t0, t0, t1
sw t0, i
j WHILE2
lab END2
stp
#______________
