.text


	li $t0, 0x00800000
	li $t1, 0x00a00000
	
	mult $t0, $t1
	
	mflo $t2
	mfhi $t3