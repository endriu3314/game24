/* file : helpers.c */
/* author(s) : A. R. Smaranda (a.r.smaranda@student.rug.nl) */
/* Description: Multiple helpful functions */

#include "helpers.h"

#include <stdio.h>

/**
 * Print array on screen
 * @param array
 * @param size
 */
void printArray(int *array, int size) {
    for (int idx = 0; idx < size; idx++) {
        printf("%d", array[idx]);

        if (idx != (size - 1)) {
            printf(" ");
        }
    }
}