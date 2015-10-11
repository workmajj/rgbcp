#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint8_t r, g, b;

    char curr;
    uint8_t step = 0;

    unsigned int count = 0;
    unsigned int frame = 1;

    while (1) {
        if (count == 0) {
            printf("FRAME\t%d\n", frame);
        }

        curr = fgetc(stdin);

        switch (step) { // TODO: enum
        case 0:
            // throwaway x for 0rgb format
            step++;
            break;
        case 1:
            r = curr;
            step++;
            break;
        case 2:
            g = curr;
            step++;
            break;
        case 3:
            b = curr;
            step = 0;
            break;
        default:
            // not reached
            break;
        }

        // printf("%u %u %u ", r, g, b);

        count++;
        if (count == 320 * 240 * 4) {
            // printf("----------------------------------------\n\n\n\n\n\n");

            count = 0;
            frame++;
        }
    }

    // fclose(fin);

    return 0;
}
