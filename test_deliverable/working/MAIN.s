	.text

#----------FUNCTION main----------
	.globl	main
	.ent	main
	.type	main, @function
main:
#ALLOCATING STACK
	addiu	$sp, $sp,-78
	sw	$ra,74($sp)
	sw	$fp,70($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 0
	b main_function_end_1	 #return

#Current Scope Level = 0
main_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,74($sp)
	nop
	lw	$fp,70($sp)
	nop
	addiu	$sp, $sp,78
	j	$ra
	nop

	.end	main

