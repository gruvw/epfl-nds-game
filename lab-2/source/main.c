/*
 * Template Nintendo DS
 * May 2011
 */
#include <nds.h>
#include <stdio.h>

int main(void) {
    consoleDemoInit();

    /* Exercise 1 */

    printf("\nHello this is me Mario");

    /* Exercise 2 */

    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int min = array[0];  // error if array is empty as there is no min/max
    int max = array[0];
    int sum = 0;
    int length = sizeof(array) / sizeof(*array);

    for (size_t i = 0; i < length; i++) {
        min = array[i] < min ? array[i] : min;
        max = array[i] > max ? array[i] : max;
        sum += array[i];
    }

    int average = sum / length;

    printf("\nMin: %d, Max: %d, Average: %d\n\n", min, max, average);

    /* Exercise 3 */

    const int SIZE = 3;
    int matrix[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i * SIZE + j]);
        }
        printf("\n\n");
    }

    while (1) {
        swiWaitForVBlank();
    }
}
