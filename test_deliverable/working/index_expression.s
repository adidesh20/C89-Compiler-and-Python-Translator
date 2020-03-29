	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-118
	sw	$ra,114($sp)
	sw	$fp,110($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable i


#local array

	li	$v0, 0
#local var counter = 10
	sw	$v0, 52($fp)	#Storing variable acc


	li	$v0, 8
	sw	$v0, 16($fp)		#assigning value 8 to local variable i
for_loop_0_begin:	#Begin for loop
#lock $t0
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: i
	nop
	li	$t0, 16
	slt	$v0, $v0, $t0		 #comparison (<)
#unlock $t0
	beq	$0, $2, end_loop_0
	nop

#Current Scope Level = 2
#lock $t0
	 #varAddress = 16
	lw	$t0, 16($fp)		 #loading local (scope) variable: i
	nop
	sw	$8, 20($fp)	#Assign element 0 of array x
	nop
#unlock $t0

#Current Scope Level = 1
for_increment_0:	#Increment stage of for loop
#lock $t0
	lw	$t0, 16($fp)		 #loading local variable i
	nop
	addiu	$t0, $t0, 1		 #incrementing by 1 due to ++ operator
	sw	$t0, 16($fp)		 #storing i after increment
#unlock $t0
	b	for_loop_0_begin
	nop
end_loop_0:	#End for loop

	li	$v0, 0
	sw	$v0, 52($fp)		#assigning value 0 to local variable acc

	li	$v0, 0
	sw	$v0, 16($fp)		#assigning value 0 to local variable i
for_loop_1_begin:	#Begin for loop
#lock $t0
	 #varAddress = 16
	lw	$v0, 16($fp)		 #loading local (scope) variable: i
	nop
	li	$t0, 8
	slt	$v0, $v0, $t0		 #comparison (<)
#unlock $t0
	beq	$0, $2, end_loop_1
	nop

#Current Scope Level = 2
#lock $t0
#lock $t1
	 #varAddress = 52
	lw	$t0, 52($fp)		 #loading local (scope) variable: acc
	nop
	lw	$t1, 20($fp)		#Accessing index 0 of array x
	nop
	addu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
	sw	$v0, 52($fp)		#assigning value 0 to local variable acc

#Current Scope Level = 1
for_increment_1:	#Increment stage of for loop
#lock $t0
	lw	$t0, 16($fp)		 #loading local variable i
	nop
	addiu	$t0, $t0, 1		 #incrementing by 1 due to ++ operator
	sw	$t0, 16($fp)		 #storing i after increment
#unlock $t0
	b	for_loop_1_begin
	nop
end_loop_1:	#End for loop

	 #varAddress = 52
	lw	$v0, 52($fp)		 #loading local (scope) variable: acc
	nop
	b f_function_end_1	 #return

#Current Scope Level = 0
f_function_end_1:
#DEALLOCATING STACK
	move	$sp, $fp
	lw	$ra,114($sp)
	nop
	lw	$fp,110($sp)
	nop
	addiu	$sp, $sp,118
	j	$ra
	nop

	.end	f

