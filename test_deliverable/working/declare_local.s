	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-114
	sw	$ra,110($sp)
	sw	$fp,106($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable y


#local array

	li	$v0, 13
	sw	$v0, 16($fp)		#assigning value 13 to local variable y

	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: y
	nop
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,110($sp)
	nop
	lw	$fp,106($sp)
	nop
	addiu	$sp, $sp,114
	j	$ra
	nop

	.end	f

