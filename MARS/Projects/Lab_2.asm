
#
# 	3DR4 Lab #2: FP_MULT function
#
#	Victor Skvortsov #400010146

.data
	message1: 	.asciiz "\nResult from MIPS instruction MUL.S is: "
	message2: 	.asciiz "\nResult from FP_MULT instruction is: "
	message3:	.asciiz "\nCarry Necessary\n"
	message4: 	.asciiz "\n Enter the input values\n"

	N1:		.float 1.5
	N2:		.float 1.5
	
.text
	.globl main
main:
	la 		$a0, message4 		
	li 		$v0, 4       		#syscall to print string
	syscall
	
	li 		$v0, 6       		#syscall to read float
	syscall	
	swc1 		$f0, N1			#get the first value

	li 		$v0, 6       		#syscall to read float
	syscall		
	swc1 		$f0, N2			#get the second value

	#perform the regular MIPS float point multiplication instruction
	lwc1		$f0, N1			#load the first float
	lwc1		$f1, N2			#load the second float
	mul.s 		$f2, $f0, $f1		#use the MIPS single percision instruction
	
	li 		$v0, 4			#print message call
	la		$a0, message1
	syscall	
	
	li		$v0, 2			#print fp call
	mov.s		$f12, $f2
	syscall
	
	# now call FP_MULT instruction
	mfc1		$a0, $f0		#copy first float into argument 1
	mfc1		$a1, $f1		#copy second float into argument 2
	jal		FP_MULT
	
	mtc1		$v0, $f2		#copy the returned value to float register

	#after returning
	li		$v0, 4			#print message system call
	la 		$a0,message2
	syscall
	
	li 		$v0, 2			#print float call
	mov.s		$f12, $f2
	syscall
													
done:
	li $v0, 10			#exit program
	syscall

FP_MULT:
	andi		$t0, $a0, 0x7F800000	#extract the 8 bit exponent from argument 1
	andi 		$t1, $a1, 0x7F800000	#extract the 8 bit exponent from argument 2
	
	srl		$t0,$t0, 23		# shift exponent 1 right, 23 bits
	srl		$t1,$t1, 23		# shift exponent 2 right, 23 bits
	
	#take care of the bias
	sub 		$t6, $t0, 127		#remove bias from first argument
	sub		$t7, $t1, 127		#remove bias from second argument	
	add		$t4, $t6, $t7		#add the exponents together
	
	addi		$t4, $t4, 127		#add the bias back in 
	
	andi		$t2, $a0, 0x007FFFFF	#extract the 23 bit significand from argument 1
	ori		$t2, $t2, 0x00800000	#add the implied 24 bit
		
	andi		$t3, $a1, 0x007FFFFF	#extract the 23 bit significand from argument 2
	ori		$t3, $t3, 0x00800000	#add the implied 24 bit			
	
	multu		$t2, $t3		#multiply the significands together	
	mfhi		$t5			#get the High result	
	blt 		$t5, 0x00008000, noModify	#check for overflow, check if the aanswer requires a point carry, jump to noModify if it doesn't
	
	addi 		$t4, $t4, 1			#answer requires a point carry, modify the exponant	
	sll		$t5,$t5,15		#shift the high value over to the right
	mflo		$t9			#get the low value
	srl		$t9,$t9,16		#shift the low values all the way to the left	
	or		$t5, $t5,$t9		#or the values together	
	srl		$t5,$t5,7		#shift the product to the float point percision fraction location	
	
	j finish
	
noModify:
	sll		$t5,$t5,16		#shift the high value over to the right
	mflo		$t9			#get the low value
	srl		$t9,$t9,16		#shift the low values all the way to the left	
	or		$t5, $t5,$t9		#or the values together	
	srl		$t5,$t5,7		#shift the product to the float point percision fraction location	
	
finish:
	#compare the signs
	
	or 		$t1, $a0, $a1
	andi 		$t1, $t1, 0x80000000		#test for the sign bit
	
	andi		$t5,$t5,0x007FFFFF	# remove leading `1'	
	sll		$t4,$t4,23		# shift exponent up 23 bits
	or		$v0,$t4,$t5		# OR exponent with significand
	or 		$v0, $v0, $t1		#or the anwer with the sign
	jr		$ra				# return to caller	
	
