	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp

#Current Scope Level = 1
while_loop_0_begin:	#Begin while loop
	li	$v0, 0
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
#Current Scope Level = 1
	b	while_loop_0_begin
	nop
end_loop_0:	#End while loop

	li	$v0, 19937
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,74($sp)
	nop
	lw	$fp,70($sp)
	nop
	addiu	$sp, $sp,78
	j	$ra
	nop

	.end	f

