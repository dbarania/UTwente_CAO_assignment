#------------------------------------------------------------------------------
# Program calculating Haming distance between set of strings
# Author: Damian Baraniak and Camilo Sebastan Lozoya Enriquez
# Date: September 2024
#------------------------------------------------------------------------------

.data
	#--------------------------------------------------------------------------
	# Sets of binary strigns are read from the user as ASCII characters.
	# Representing data as ASCCII characters allows convinient use of "0" 
	# and "1", because one bit difference in binary representation.
	# Using different characters is allowed and program will calculate the
	# Hamming distance based on their binary representation.
	#--------------------------------------------------------------------------
	LengthOfSequences: .word 5
	NumberOfSequences: .word 4

	# Storing strings in the continous memory allows use of pointer arithmetics
	buffer: .space 20 # Must be LengthOfSequences * NumberOfSequences bytes
	
.text
main:
	# To avoid constant loading of L and N, they are stored in the $s1 ansd $s2
	lw $s1, LengthOfSequences
	lw $s2, NumberOfSequences
	
	#--------------------------------------------------------------------------
	# Reading all the stings from the user in the loop
	# $t1 - a counter, when 0, exit the loop.
	# $a0 - a pointer where to store the read string,
	# 	it is increased by L in each iteration
	# $a1 - a constant length of the string, due to null operator it is L+1
	#--------------------------------------------------------------------------

	la $a0, buffer
	addi $v0, $zero, 8
	addi $a1, $s1, 1
	add $t1, $s2, $zero
	
	readString:
		syscall
		addi $t1, $t1, -1
		add $a0, $a0, $s1
		bgtz $t1, readString
	

	#--------------------------------------------------------------------------
	# Calculation of the Hamming distance
	# 
	# Registers:
	# $s0 - result of calculations
	# $t1 - pointer to the "main" byte in the comparison
	# $t2 - pointer to the "secondary" byte in the comparison
	# $t3 - pointer to the next byte after buffer,to compare if pointers
	# go out of bounds
	# $t4, $t5 - Values of blocks pointed by $t1 and $t2 respectively
	# 
	# $t0 - helper register to store result of the XOR
	# $t6 - helper register to help with counting 1's
	#--------------------------------------------------------------------------
	
	la $t1, buffer
	add $t3, $a0, $zero # Adress after the buffer got while reading from user
	lb $t4, ($t1) # Loading data from the first byte of the buffer
	add $s0, $zero, $zero # Initializing result register

	NewMainBlock:
		#----------------------------------------------------------------------
		# Data is of the same size and stored in the continous memory, getting
		# address of the secondary block is done by moving $t1 or $t2 by L.
		#----------------------------------------------------------------------
		add $t2, $t1, $s1  

		ComputeHD:	
			# When $t2 points out of the buffer new main block is calculated
			slt $at, $t2, $t3
			beq $at, $zero SecondaryBlockTooBig 

			lb $t5, ($t2)
			xor $t0, $t4, $t5
			#------------------------------------------------------------------
			# Counting 1's in the $t0 is done by extracting the LSB, adding it 
			# to the result and then shifting $t0 right by 1.
			# The loop runs if $t0 != 0.
			#------------------------------------------------------------------
			CountingBits:
				andi $t6, $t0, 1
				add $s0, $s0, $t6
				srl $t0, $t0, 1
				beq $t0, $zero, UpdateSecondBlock
				# To avoid using J instructions, trivial branches are used
				beq $zero, $zero, CountingBits
		
		UpdateSecondBlock:
			# Increasing $t2 to point to the block in the next string
			add $t2, $t2, $s1 
			# To avoid using J instructions, trivial branches are used
			beq $zero, $zero, ComputeHD 
	
		#----------------------------------------------------------------------
		# When $t2 poits out of buffer this means new main block needs to be 
		# calculated, we do this by adding 1 to the current address of $t1
		#----------------------------------------------------------------------
		SecondaryBlockTooBig:
		addi $t1, $t1, 1
		lb $t4, ($t1)

		# Checking if new main block is not out of buffer
		slt $at, $t3, $t1
		beq $at, $zero, NewMainBlock
	
	# Exit
	addi $v0, $zero, 10
	syscall
