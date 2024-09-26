.data
	LengthOfSequences: .word 5
	NumberOfSequences: .word 5
	buffor: .space 25
	
.text
main:
	lw $s1, LengthOfSequences
	lw $s2, NumberOfSequences
	
	la $a0, buffor
	addi $v0, $zero, 8
	addi $a1, $s1, 1
	move $t1, $s2
	
	readString:
		syscall
		addi $t1, $t1, -1
		add $a0, $a0, $s1
		bgtz $t1, readString
	
	la $t1, buffor
	move $t3, $a0
	lb $t4, ($t1)
	add $s0, $zero, $zero
	Loop1:
		add $t2, $t1, $s1
		ComputeHD:
			bge $t2, $t3, Label 
			lb $t5, ($t2)
			xor $t0, $t4, $t5
			CountingBits:
				andi $t6, $t0, 1
				add $s0, $s0, $t6
				srl $t0, $t0, 1
				bgtz $t0, CountingBits
			add $t2, $t2, $s1
			bge $zero, $zero, ComputeHD
		Label:
		addi $t1, $t1, 1
		lb $t4, ($t1)
		bgt $t3, $t1, Loop1
	addi $v0, $zero, 10
	syscall
