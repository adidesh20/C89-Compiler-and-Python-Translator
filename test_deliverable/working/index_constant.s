	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-110
	sw	$ra,106($sp)
	sw	$fp,102($sp)
	move	$fp, $sp

#Current Scope Level = 1
#local array

#lock $t0
	li	$t0, 23
	sw	$8, 16($fp)	#Assign element 0 of array x
	nop
#unlock $t0

	lw	$v0, 16($fp)		#Accessing index 0 of array x
	nop
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,106($sp)
	nop
	lw	$fp,102($sp)
	nop
	addiu	$sp, $sp,110
	j	$ra
	nop

	.end	f

