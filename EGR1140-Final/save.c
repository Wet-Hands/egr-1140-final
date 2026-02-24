#include <stdio.h>
#include "save.h"

// XOR key used to obfuscate the score value on disk.
// Change this to any non-zero number to use a different key.
#define XOR_KEY 0xDD

// Checksum: sum of each byte of the XOR'd value, masked to 8 bits.
// Written alongside the score so we can detect file tampering on load.
static unsigned char compute_checksum(unsigned int xored)
{
    unsigned char sum = 0;
    // Walk each byte of the 4-byte integer
    sum += (xored      ) & 0xFF;
    sum += (xored >>  8) & 0xFF;
    sum += (xored >> 16) & 0xFF;
    sum += (xored >> 24) & 0xFF;
    return sum;
}

// Writes two space-separated hex values to save.txt:
//   <xor'd score>  <checksum>
// Example: a score of 10 with XOR_KEY 0xDD becomes "D7 6D" on disk.
void save_highscore(int score)
{
    int current = load_highscore();
    if (score <= current) return;

    unsigned int xored    = (unsigned int)score ^ XOR_KEY;
    unsigned char chk     = compute_checksum(xored);

    FILE *f = fopen("do_not_open.txt", "w");
    FILE *ff = fopen("save.txt", "w");
    if (f != NULL)
    {
        fprintf(f, "%X %X", xored, (unsigned int)chk);
        fprintf(ff, "%X %X", xored, (unsigned int)chk);
        fclose(f);
    }
}

// Reads and validates the save file.
// Returns the decrypted score, or 0 if the file is missing or tampered with.
int load_highscore(void)
{
    unsigned int xored = 0;
    unsigned int chk_stored = 0;

    FILE *f = fopen("do_not_open.txt", "r");
    if (f == NULL) return 0;

    int read = fscanf(f, "%X %X", &xored, &chk_stored);
    fclose(f);

    // Need both values; reject if file is malformed
    if (read != 2) return 0;

    // Recompute checksum and compare — reject if tampered
    unsigned char chk_expected = compute_checksum(xored);
    if ((unsigned int)chk_expected != chk_stored) return 0;

    // Decrypt and return
    return (int)(xored ^ XOR_KEY);
}
