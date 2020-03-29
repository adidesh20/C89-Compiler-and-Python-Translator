	.text

#----------FUNCTION log_not_fun----------
	.globl	log_not_fun
	.ent	log_not_fun
	.type	log_not_fun, @function
log_not_fun:
#ALLOCATING STACK
	addiu	$sp, $sp,-90
	sw	$ra,86($sp)
	sw	$fp,82($sp)
	move	$fp, $sp

#Current Scope Level = 1
	li	$v0, 10
#local var counter = 1
	sw	$v0, 16($fp)	#Storing variable a


	li	$v0, 10
#local var counter = 2
	sw	$v0, 20($fp)	#Storing variable b


	li	$v0, 15
#local var counter = 3
	sw	$v0, 24($fp)	#Storing variable c


#lock $t0
