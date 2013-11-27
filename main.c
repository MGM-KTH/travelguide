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

    int tour[nodecount];

    int i;
    int j;

    for(i = 0; i < nodecount; ++i) {
        scanf("%f %f", &x[i], &y[i]);
    }

    //print_farray(x, nodecount);
    //print_farray(y, nodecount);

    int distances_size = nodecount*(nodecount-1)/2;
    int distances[distances_size];

    // Calculate pairwise distances
    for(i = 0; i < nodecount; ++i) {
        for(j = i+1; j < nodecount; ++j) {
            distances[get_index(i,j)] = distance(x[i], y[i], x[j], y[j]);
        }
    }

    //print_iarray(distances, distances_size);
    //print_diag_matrix(distances, nodecount);

    tsp(distances, tour, nodecount);
    print_tour(tour, nodecount);

    return 0;
}

void tsp(int distances[], int tour[], int nodecount) {
    int used[nodecount];
    int best;
    int i, j;
    tour[0] = 0;
    used[0] = 1;
    // Initialize used (visited) array
    for (i = 1; i < nodecount; ++i) {
        used[i] = 0;
    }
    // Nearest neighbour
    for(i = 1; i < nodecount; ++i) {
        best = -1;
        for(j = 0; j < nodecount; ++j) {
            if(used[j] == 0 && (best == -1 || distances[get_index(tour[i-1],j)] < distances[get_index(tour[i-1],best)])) {
                best = j;
            }
        }
        tour[i] = best;
        used[best] = 1;
    }
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
    for(i = 1; i < length; ++i) {
        printf(", %d", array[i]);
    }
    printf("]\n");
}

void print_tour(int array[], int length) {
    int i;
    for(i = 0; i < length; ++i) {
        printf("%d\n", array[i]);
    }
}

void print_diag_matrix(int matrix[], int nodecount) {
    int i;
    for(i = 1; i < nodecount; ++i) {
        print_iarray(&matrix[get_index(i, 0)], i);
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
