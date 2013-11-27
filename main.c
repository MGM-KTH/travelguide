#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "main.h"

/* Prototypes */
static int distance(float x1, float y1, float x2, float y2);
static int get_index(int a, int b);


int main(int argc, char *argv[]) {
    int nodecount;
    scanf("%d", &nodecount);

    float x[nodecount];
    float y[nodecount];

    int i;
    int j;

    for(i = 0; i < nodecount; ++i) {
        scanf("%f %f", &x[i], &y[i]);
    }

    print_farray(x, nodecount);

    print_farray(y, nodecount);

    int distances_size = nodecount*(nodecount-1)/2;
    int distances[distances_size];

    for(i = 0; i < nodecount; ++i) {
        for(j = i+1; j < nodecount; ++j) {
            distances[get_index(i,j)] = distance(x[i], y[i], x[j], y[j]);
        }
    }

    print_iarray(distances, distances_size);
    print_diag_matrix(distances, nodecount);

    return 0;
}

void print_farray(float array[], int length) {
    int i;
    printf("[%f", array[0]);
    for(i = 1; i < length; i++) {
        printf(", %f", array[i]);
    }
    printf("]\n");
}

void print_iarray(int array[], int length) {
    int i;
    printf("[%d", array[0]);
    for(i = 1; i < length; i++) {
        printf(", %d", array[i]);
    }
    printf("]\n");
}

void print_diag_matrix(int matrix[], int nodecount) {
    int i;
    int j;
    for(i = 0; i < nodecount-1; ++i) {
        j += i;
        print_iarray(&matrix[j], i);
    }
}

int distance(float x1, float y1, float x2, float y2) {
    return floor(sqrt(pow(x1-x2, 2) + pow(y1-y2, 2)) + 0.5);
}

int get_index(int a, int b) {
    // We do not store the trivial 0-distances x --> x
    assert(a != b);
    // We only store the distance in one direction, a --> b since it's
    // Euclidean distances. i.e. a --> b = b --> a.
    // Swap a and b if b > a.
    if (b > a) {
        int temp = a;
        a = b;
        b = temp;
    }
    // Calculate the array index using the formula for summing the first x integers.
    return a*(a-1)/2+b;
}
