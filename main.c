#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

/* 
    Efficiency of calculation (time and memory wise) depends on 
    the size of baisc block. Using macros, size of the block can be set 
    at the compile time, it should be unsinged value.
    By default block is set to uint16_t.
*/
#ifndef BLOCK_TYPE
#define BLOCK_TYPE uint16_t
#endif
typedef BLOCK_TYPE block;
block msbExtraction = (block)1 << (8 * sizeof(block) - 1);

/*
    Random generation of binary strings assumes that rand() returns 31 bits
    sequences, to properly fill 32 bits, second value needs to be generated,
    msb extracted and appended to the from of the first values using bitwise
    operations.

    Length of the sequence doesn't have to be a multiple of a size of a block,
    in that case we need additional block to store the remaining part. 
    Only remaining most significant bits are filled, rest is filled with 0's.
*/
void generateRandomSequence(block *startPtr,
                            const uint32_t numberOfBlocks,
                            const int32_t sequenceLength)
{

    for (size_t i = 0; i < numberOfBlocks; i++)
    {
        startPtr[i] = ((rand() << 1) & msbExtraction) | rand();
    }

    block alignBits = sequenceLength % (8 * sizeof(block));

    if (alignBits)
    {
        uint16_t diff = 8 * sizeof(block) - alignBits;
        startPtr[numberOfBlocks - 1] <<= diff;
    }
}

/*
    Calculating hamming distance between 2 binary strings comes down to
    comparing every pair of block with XOR and counting the 1's.
*/
void getHammingWeight(const block *sequence1,
                      const block *sequence2,
                      uint32_t *resultPtr,
                      const uint32_t numberOfBlocks)
{

    uint32_t localResult = 0;

    for (int32_t i = 0; i < numberOfBlocks; i++)
    {
        block xorResult = sequence1[i] ^ sequence2[i];
        while (xorResult)
        {
            xorResult &= xorResult - 1;
            localResult++;
        }
    }

    *resultPtr += localResult;
}

int main(int argc, char **argv)
{
    int32_t numberOfSequences = 10;
    int32_t sequenceLength = 16;
    int32_t seed = 42;

    // Reading length and number of sequences, random seed
    char argument;
    while ((argument = getopt(argc, argv, "N:L:s:")) != -1)
    {
        switch (argument)
        {
        case 'N':
            numberOfSequences = atoi(optarg);
            break;
        case 'L':
            sequenceLength = atoi(optarg);
            break;
        case 's':
            seed = atoi(optarg);
            break;
        }
    }
    srand(seed);
    
    // Calculation sto make sure the minimum amount of blocks is used to store data
    const uint32_t numberOfBlocks = (sequenceLength + 8 * sizeof(block) - 1) / (8 * sizeof(block));
    
    // Allocating memory for the data
    block **array = (block **)malloc(numberOfSequences * sizeof(block *));
    for (int32_t i = 0; i < numberOfSequences; i++)
    {
        array[i] = (block *)malloc(numberOfBlocks * sizeof(block));
    }

    for (int32_t i = 0; i < numberOfSequences; i++)
    {
        generateRandomSequence(array[i], numberOfBlocks, sequenceLength);
    }

    uint32_t result = 0;

    for (int32_t i = 0; i < numberOfSequences; i++)
    {
        for (int32_t j = i + 1; j < numberOfSequences; j++)
        {
            getHammingWeight(array[i], array[j], &result, numberOfBlocks);
        }
    }
    int32_t numberOfCombinations = (numberOfSequences * (numberOfSequences - 1) / 2);
    printf("Sum of distances %d, average %f", result, (float)result / numberOfCombinations);

    return 0;
}
