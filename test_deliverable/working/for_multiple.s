	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-86
	sw	$ra,82($sp)
	sw	$fp,78($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable x


	li	$v0, 0
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable y


	li	$v0, 0
	sw	$v0, 20($fp)		#assigning value 0 to local variable y

	li	$v0, 0
	sw	$v0, 16($fp)		#assigning value 0 to local variable x
for_loop_0_begin:	#Begin for loop
#lock $t0
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: x
	nop
	li	$t0, 10
	slt	$v0, $v0, $t0		 #comparison (<)
#unlock $t0
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
#lock $t0
#lock $t1
	 #varAddress = 20
	lw	$t0, 20($fp)		 #loading local (scope) variable: y
	nop
	li	$t1, 1
	subu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	sw	$v0, 20($fp)		#assigning value -2 to local variable y

#Current Scope Level = 1
for_increment_0:	#Increment stage of for loop
#lock $t0
#lock $t1
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: x
	nop
	li	$t1, 1
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	sw	$v0, 16($fp)		#assigning value 2 to local variable x
	b	for_loop_0_begin
	nop
end_loop_0:	#End for loop

	 #varAddress = 20
	lw	$v0, 20($fp)		 #loading local (scope) variable: y
	nop
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,82($sp)
	nop
	lw	$fp,78($sp)
	nop
	addiu	$sp, $sp,86
	j	$ra
	nop

	.end	f

