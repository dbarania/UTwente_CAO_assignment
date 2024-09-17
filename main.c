#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define BLOCK_TYPE uint16_t
typedef BLOCK_TYPE block;
block msbExtraction = 1 << (8 * sizeof(block) - 1);

void block_to_binary(block num) {
    system("clear");
    // Allocate memory for 16 bits + 1 for null terminator
    char *binary_str = (char *) malloc(17);
    if (binary_str == NULL) {
        free(binary_str);
        exit(3);
    }

    // Loop through each bit (from MSB to LSB)
    for (int i = 15; i >= 0; i--) {
        binary_str[15 - i] = (num & (1 << i)) ? '1' : '0';
    }

    binary_str[16] = '\0';  // Null-terminate the string
    puts(binary_str);
    free(binary_str);
}

void
getHammingWeight(const block *sequence1, const block *sequence2, uint32_t *resultPtr, const uint32_t numberOfBlocks) {
    uint32_t localResult = 0;
    for (int32_t i = 0; i < numberOfBlocks; i++) {
        block xorResult = sequence1[i] ^ sequence2[i];
        while (xorResult) {
            xorResult &= xorResult - 1;
            localResult++;
        }
    }
    *resultPtr += localResult;
}

void generateRandomSequence(block *startPtr, const uint32_t numberOfBlocks, const int32_t sequenceLength) {
    for (size_t i = 0; i < numberOfBlocks; i++) {
        startPtr[i] = ((rand() << 1) & msbExtraction) | rand();
        //#ifndef NDEBUG
        //        block_to_binary(startPtr[i]);
        //#endif
    }
    block alignBits = sequenceLength % (8 * sizeof(block));
    if (alignBits) {
        uint16_t diff = 8 * sizeof(block) - alignBits;
        startPtr[numberOfBlocks - 1] <<= diff;
    }
}

int main(int argc, char **argv) {
    int32_t numberOfSequences = 10;
    int32_t sequenceLength = 16;
    int32_t seed = 42;

    // Reading length and number of sequences, random seed
    char argument;
    while ((argument = getopt(argc, argv, "N:L:s:")) != -1) {
        switch (argument) {
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
    block **array = (block **) malloc(numberOfSequences * sizeof(block *));
    for (int32_t i = 0; i < numberOfSequences; i++) {
        array[i] = (block *) malloc(numberOfBlocks * sizeof(block));
    }

    for (int32_t i = 0; i < numberOfSequences; i++) {
        generateRandomSequence(array[i], numberOfBlocks, sequenceLength);
    }

    uint32_t result = 0;
    for (int32_t i = 0; i < numberOfSequences; i++) {
        for (int32_t j = i + 1; j < numberOfSequences; j++) {
            getHammingWeight(array[i], array[j], &result, numberOfBlocks);
        }
    }

    printf("Sum of distances %d, average %f", result, (float) result / numberOfSequences);

    return 0;

}
