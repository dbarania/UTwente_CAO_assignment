#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#ifndef BLOCK_TYPE
#define BLOCK_TYPE uint16_t
#endif
typedef BLOCK_TYPE block;
block msbExtraction = 1 << (8 * sizeof(block) - 1);

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

    const uint32_t numberOfBlocks = (sequenceLength + 8 * sizeof(block) - 1) / (8 * sizeof(block));
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
