/*Pallav Regmi*/
/*CS-241*/
/*Lab06*/

#include <stdio.h>
#include "lcg.h"

#define FILL_M 0
#define FILL_C 1
#define FILL_DATA 2
#define ENCRYPT 1
#define DECRYPT 2

int encryptData(unsigned long m, unsigned long c, int ch, unsigned long x);
int decryptData(unsigned long m, unsigned long c, int ch, unsigned long x);

int main() {

    
    int c, currentPos, action, lineNum, error;
    unsigned long m, ci, random;
    currentPos = action = error = 0;
    m = ci = random = 0;
    lineNum = 1;

    while ((c = getchar())!= EOF) {
        if (currentPos == FILL_M && (c == 'e' || c == 'd')) {
            action = (c == 'e') ? ENCRYPT : DECRYPT;
        }

        if (c != ',' && c != '\n') {
            if (currentPos == FILL_M && c >= '0' && c <= '9') {
                m = 10 * m + (c - '0');
            } else if (currentPos == FILL_C && c >= '0' && c <= '9') {
                ci = 10 * ci + (c - '0');
            } else if (currentPos == FILL_DATA) {
                struct LinearCongruentialGenerator lcg = makeLCG(m, ci);
                printf("%5d) ", lineNum++);
                if (lcg.m == 0) {
                    printf("Error\n");
                    while ((c = getchar()) != '\n');
                } else {
                    random = getNextRandomValue(&lcg);
                    while (c != '\n' && error == 0) {
                        error = (action == ENCRYPT) ? encryptData(m, ci, c, random) : decryptData(m, ci, c, random);
                        c = getchar();
                        random = getNextRandomValue(&lcg);
                    }
                    printf("\n");
                }
                m = ci = 0;
                currentPos = FILL_M;
                action = error = 0;
            }
        }

        if (c == ',') {
            if (currentPos == FILL_M) {
                currentPos = FILL_C;
            } else if (currentPos == FILL_C) {
                currentPos = FILL_DATA;
            }
        }

        if (c == '\n' && currentPos != FILL_M) {
            printf("%5d) ", lineNum++);
            printf("Error\n");
            m = ci = 0;
            currentPos = FILL_M;
            action = 0;
        }
    }

    return 0;
}

int encryptData(unsigned long m, unsigned long c, int ch, unsigned long x) {
    int output = ch ^ (x % 128);

    if (m == 0 || c == 0) {
        printf("Error");
        return 1;
    }

    if (output < 32) {
        printf("*%c", ('?' + output));
    } else if (output == 127) {
        printf("*!");
    } else if (output == '*') {
        printf("**");
    } else {
        printf("%c", output);
    }

    return 0;
}

int decryptData(unsigned long m, unsigned long c, int ch, unsigned long x) {
    int output = ch ^ (x % 128);

    if (m == 0 || c == 0) {
        printf("Error");
        return 1;
    }

    if (ch == '*') {
        ch = getchar();
        if (ch == '*') {
            output = ch ^ (x % 128);
            printf("%c", output);
        } else if (ch == '!') {
            output = 127 ^ (x % 128);
            printf("%c", output);
        } else if (ch > 32 && ch < 127) {
            output = (ch - '?') ^ (x % 128);
            printf("%c", output);
        } else if (ch == 32) {
            printf("Error");
            return 1;
        }
    } else {
        printf("%c", output);
    }

    return 0;
}
