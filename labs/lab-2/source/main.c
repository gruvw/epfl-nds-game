#include <nds.h>
#include <stdio.h>

/* Exercise 4 */

int factorial_iter(int n) {
    int res = 1;

    for (int i = n; i >= 2; i--) {
        res *= i;
    }

    return res;
}

int factorial_rec(int n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial_rec(n - 1);
}

/* Exercise 5 */

int gcd(int a, int b) {
    int rem = a % b;

    if (!rem) {
        return b;
    }

    return gcd(b, rem);
}

/* MAIN */

int main(void) {
    consoleDemoInit();

    /* Exercise 1 */

    printf("\nHello this is me Mario\n");

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

    /* Exercise 4 */

    const int NB = 4;
    printf("Factorial of %d: iter %d, rec %d\n", NB, factorial_iter(NB), factorial_rec(NB));

    /* Exercise 5 */

    const int A = 20, B = 15;
    printf("\nGCD of %d & %d: %d", A, B, gcd(A, B));

    while (1) {
        swiWaitForVBlank();
    }
}
