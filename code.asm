# program minipas:

.data
	a:	.word	0
	vari:	.word	0
	i:	.word	0
	n:	.word	0
	x:	.space	800	# array of floats


.text
.globl main
main:
	li $t0, 48
	sw $t0, a
	li $t0, 84
	sw $t0, vari
	li $t0, 1
	sw $t0, a
	li $t0, 2
	sw $t0, i
WHILE0:
	lw $t0, i
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	lw $t0, vari
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	sle $t0, $t1, $t0
	beq $t0, $zero, END0
	lw $t0, a
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	li $t0, 10
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	slt $t0, $t1, $t0
	beq $t0, $zero, END1
	lw $t0, a
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	li $t0, 1
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	add $t0, $t1, $t0
	sw $t0, a
END1:
	lw $t0, i
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	li $t0, 1
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	add $t0, $t1, $t0
	sw $t0, i
	j WHILE0
END0:
	li $t0, 1
	sw $t0, i
WHILE2:
	lw $t0, i
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	lw $t0, vari
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	slt $t0, $t1, $t0
	beq $t0, $zero, END2
	lw $t0, i
	li $t2, 8 # calculate index offset
	mult $t0, $t2
	mflo $t0
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	lw $t0, n
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	lw $t0, i
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	li $t0, 1
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	add $t0, $t1, $t0
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	sub $t0, $t1, $t0
	addi $sp, $sp, -8	# push
	swc1 $f0, 0($sp)
	li.s $f0, 3.140000
	lwc1 $f1, 0($sp)	# pop
	addi $sp, $sp, 8
	mul.s $f0, $f1, $f0
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	la $t4, x
	add $t3, $t4, $t1 # set new index in the array
	swc1 $f0, 0($t3)
	li $t0, 1
	li $t2, 8 # calculate index offset
	mult $t0, $t2
	mflo $t0
	addi $sp, $sp, -4	# push
	sw $t0, 0($sp)
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	la $t4, x
	add $t3, $t4, $t1 # set new index in the array
	swc1 $f0, 0($t3)
	addi $sp, $sp, -8	# push
	swc1 $f0, 0($sp)
	li $t0, 1
	lw $t1, 0($sp)	# pop
	addi $sp, $sp, 4
	add $t0, $t1, $t0
	sw $t0, i
	j WHILE2
END2:


#______________
