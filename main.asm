.data
	LengthOfSequences: .word 4
	NumberOfSequences: .word 3
	buffor: .space 12

.text
main:
	lw $s1, LengthOfSequences
	lw $s2, NumberOfSequences
	
	la $a0, buffor
	addi $v0, $zero, 8
	addi $a1, $s1, 1
	add $t1, $s2, $zero
	
	readString:
		syscall
		addi $t1, $t1, -1
		add $a0, $a0, $s1
		bne $t1, $zero, readString
	
	la $t1, buffor
	add $t3, $a0, $zero
	lb $t4, ($t1)
	add $s0, $zero, $zero
	
	moveMainByte:
		add $t2, $t1, $s1
		ComputeHD:
			bge $t2, $t3, StopComputations
			lb $t5, ($t2)
			xor $t0, $t4, $t5
			CountingBits:
				andi $t6, $t0, 1
				add $s0, $s0, $t6
				srl $t0, $t0, 1
				bne $t0, $zero, CountingBits
			add $t2, $t2, $s1
			bge $zero, $zero ComputeHD
		addi $t1, $t1, 1
		lb $t4, ($t1)
		bgt $t3, $t1, moveMainByte
	StopComputations:
	addi $v0, $zero, 10
	syscall