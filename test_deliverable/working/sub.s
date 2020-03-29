	.text

#----------FUNCTION f----------
	.globl	f
	.ent	f
	.type	f, @function
f:
#ALLOCATING STACK
	addiu	$sp, $sp,-82
	sw	$ra,78($sp)
	sw	$fp,74($sp)
	move	$fp, $sp
	sw	$a0, 82($fp)	#storing param argument register
	sw	$a1, 86($fp)	#storing param argument register
	sw	$a2, 90($fp)	#storing param argument register
	sw	$a3, 94($fp)	#storing param argument register

#Current Scope Level = 1
	li	$v0, 0
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable z


#lock $t0
#lock $t1
	 #varAddress = 0
	lw	$t0, 82($fp)		 #variable x is a function parameter
	nop
	 #varAddress = 0
	lw	$t1, 86($fp)		 #variable y is a function parameter
	nop
	subu	$v0, $t0, $t1
#unlock $t0
#unlock $t1
