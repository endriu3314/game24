/* file : main.c */
/* author(s) : A. Croitoru-Rusan (a.croitoru-rusan@student.rug.nl) &
               A. R. Smaranda (a.r.smaranda@student.rug.nl) */
/* Description: Main file of the program. */

#include <stdio.h>

#include "boolean.h"
#include "helpers.h"

// Function declarations
float calculate(int number1, int number2, char operator);
bool computeGameOf24(int *numbers, int numbersSize);

int main() {
    int numbers[4];
    for (int idx = 0; idx < 4; idx++) {
        int scan;

        scanf("%d", &scan);

        // we only accept inpus between 1 and 10
        while (scan != -1) {
            if (scan > 0 && scan < 11) {
                break;
            }

            printf("Please input a number between 1-10. \n");
            printf("Input -1 to stop \n");

            scanf("%d", &scan);
        }

        if (scan == -1) {
            return 0;
        }

        numbers[idx] = scan;
    }

    // if no way to reach 24 was found we let the user know that
    if (!computeGameOf24(numbers, 4)) {
        printf("No solution exists for numbers ");
        printArray(numbers, 4);
        printf(".");
    }

    printf("\n");
    return 0;
}

/**
 * Function to calculate an operation between 2 number
 * @param number1
 * @param number2
 * @param operator Operator to use for the calculation, one of the following: +,-,*,/
 * @return The value it calculated
 */
float calculate(int number1, int number2, char operator) {
    switch (operator) {
        case '+':
            return number1 + number2;

        case '-':
            return number1 - number2;

        case '*':
            return number1 * number2;

        case '/':
            // we can't divide by 0
            if (number2 == 0) {
                return -1000;
            }

            // we do this to make sure we don't false trigger values like 24.5 as true
            if ((float) number1 / number2 != number1 / number2) {
                return -1000;
            }

            return (float) number1 / number2;

        default:
            return 0;
    }
}

/**
 * Function that finds a way to get to 24 only by using +,-,*,/ operations
 * @param numbers Numbers array @example: [1, 2, 3, 4]
 * @param numbersSize Size of numbers array, usually it should be 4 but it's not assumed
 * @return true if a way to write 24 is found, false if there is no way to write 24
 */
bool computeGameOf24(int *numbers, int numbersSize) {
    int operatorsSize = 4;
    char operators[4] = {'+', '-', '*', '/'};

    float cal[3];
    int signs[3];// store either 0 for +, - or 1 for *, /

    // choose numbers location
    for (int permutation1 = 0; permutation1 < numbersSize; permutation1++) {

        // start permutation 2
        for (int permutation2 = 0; permutation2 < numbersSize; permutation2++) {
            // skip if we try to use same number
            if (permutation2 == permutation1) {
                continue;
            }

            // start permutation 3
            for (int permutation3 = 0; permutation3 < numbersSize; permutation3++) {
                // skip if we use same number
                if (permutation3 == permutation1 || permutation3 == permutation2) {
                    continue;
                }

                // 3 + 2 + 1 = 6 biggest value
                int permutation4 = 6 - (permutation1 + permutation2 + permutation3);

                // choose operators location
                for (int operator1 = 0; operator1 < operatorsSize; operator1++) {
                    // same process but for operators
                    signs[0] = 0;
                    if (operator1 <= 1) {
                        signs[0] = 1;
                    }

                    for (int operator2 = 0; operator2 < operatorsSize; operator2++) {
                        signs[1] = 0;
                        if (operator2 <= 1) {
                            signs[1] = 1;
                        }

                        for (int operator3 = 0; operator3 < operatorsSize; operator3++) {
                            signs[2] = 0;
                            if (operator3 <= 1) {
                                signs[2] = 1;
                            }

                            // choose operator order
                            // have different operator like '+' and '/'
                            if ((signs[0] ^ signs[1]) || (signs[0] ^ signs[2]) || (signs[1] ^ signs[2])) {
                                /**
                                 * ( (permutation1 + permutation2) * permutation3 ) / permutation4
                                 */
                                cal[0] = calculate(numbers[permutation1], numbers[permutation2], operators[operator1]);
                                cal[1] = calculate(cal[0], numbers[permutation3], operators[operator2]);
                                cal[2] = calculate(cal[1], numbers[permutation4], operators[operator3]);

                                if (cal[2] == 24) {
                                    printf("((%d %c %d) %c %d) %c %d",
                                           numbers[permutation1],
                                           operators[operator1],
                                           numbers[permutation2],
                                           operators[operator2],
                                           numbers[permutation3],
                                           operators[operator3],
                                           numbers[permutation4]);
                                    return true;
                                }

                                /**
                                 * ( permutation1 + permutation2) * (permutation3 - permutation4)
                                 */
                                cal[0] = calculate(numbers[permutation1], numbers[permutation2], operators[operator1]);
                                cal[2] = calculate(numbers[permutation3], numbers[permutation4], operators[operator3]);
                                cal[1] = calculate(cal[0], cal[2], operators[operator2]);

                                if (cal[1] == 24) {
                                    printf("(%d %c %d) %c (%d %c %d)", numbers[permutation1],
                                           operators[operator1],
                                           numbers[permutation2],
                                           operators[operator2],
                                           numbers[permutation3],
                                           operators[operator3],
                                           numbers[permutation4]);
                                    return true;
                                }

                                /**
                                 * ( permutation1 * (permutation2 + permutation3) ) / permutation4
                                 */
                                cal[1] = calculate(numbers[permutation2], numbers[permutation3], operators[operator2]);
                                cal[0] = calculate(numbers[permutation1], cal[1], operators[operator1]);
                                cal[2] = calculate(cal[0], numbers[permutation4], operators[operator3]);

                                if (cal[2] == 24) {
                                    printf("(%d %c (%d %c %d)) %c %d",
                                           numbers[permutation1],
                                           operators[operator1],
                                           numbers[permutation2],
                                           operators[operator2],
                                           numbers[permutation3],
                                           operators[operator3],
                                           numbers[permutation4]);
                                    return true;
                                }

                                /**
                                 * permutation1 / ( (permutation2 + permutation3) * permutation4 )
                                 */
                                cal[1] = calculate(numbers[permutation2], numbers[permutation3], operators[operator2]);
                                cal[2] = calculate(cal[1], numbers[permutation4], operators[operator3]);
                                cal[0] = calculate(numbers[permutation1], cal[2], operators[operator1]);

                                if (cal[0] == 24) {
                                    printf("%d %c ((%d %c %d) %c %d)",
                                           numbers[permutation1],
                                           operators[operator1],
                                           numbers[permutation2],
                                           operators[operator2],
                                           numbers[permutation3],
                                           operators[operator3],
                                           numbers[permutation4]);
                                    return true;
                                }

                                /**
                                 * permutation1 / ( permutation2 * (permutation3 + permutation4) )
                                 */
                                cal[2] = calculate(numbers[permutation3], numbers[permutation4], operators[operator3]);
                                cal[1] = calculate(numbers[permutation2], cal[2], operators[operator2]);
                                cal[0] = calculate(numbers[permutation1], cal[1], operators[operator1]);

                                if (cal[0] == 24) {
                                    printf("%d %c (%d %c (%d %c %d))",
                                           numbers[permutation1],
                                           operators[operator1],
                                           numbers[permutation2],
                                           operators[operator2],
                                           numbers[permutation3],
                                           operators[operator3],
                                           numbers[permutation4]);
                                    return true;
                                }
                            } else {
                                cal[0] = calculate(numbers[permutation1], numbers[permutation2], operators[operator1]);
                                cal[1] = calculate(cal[0], numbers[permutation3], operators[operator2]);
                                cal[2] = calculate(cal[1], numbers[permutation4], operators[operator3]);

                                if (cal[2] == 24) {
                                    printf("%d %c %d %c %d %c %d",
                                           numbers[permutation1],
                                           operators[operator1],
                                           numbers[permutation2],
                                           operators[operator2],
                                           numbers[permutation3],
                                           operators[operator3],
                                           numbers[permutation4]);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // if no value was found to reach 24 we return false
    return false;
}