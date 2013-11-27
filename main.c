#include "main.h"
#include <assert.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int nodecount;
    scanf("%d", &nodecount);

    float x[nodecount];
    float y[nodecount];

    int i;

    for(i = 0; i < nodecount; i++) {
        scanf("%f %f", &x[i], &y[i]);
    }

    print_array(x, nodecount);

    print_array(y, nodecount);

    int distances[nodecount*(nodecount-1)/2];

    return 0;
}

void print_array(float array[], int length) {
    int i;
    printf("[%f", array[0]);
    for(i = 1; i < length; i++) {
        printf(", %f", array[i]);
    }
    printf("]\n");
}

int get_index(int a, int b) {
    // We do not store the trivial 0-distances x --> x
    assert(a != b);
    // We only store the distance in one direction, a --> b since it's
    // Euclidean distances. i.e. a --> b = b --> a.
    // Swap a and b if b > a.
    if (b > a) {
        temp = a;
        a = b;
        b = temp;
    }
    // Calculate the array index using the formula for summing the first x integers.
    return a*(a-1)/2+b;
}
