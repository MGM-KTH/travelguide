#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "main.h"

/* Prototypes */
static int distance(float x1, float y1, float x2, float y2);
static int get_index(int a, int b);
int tsp(int distances[], int tour[], int nodecount);
void two_opt(int distances[], int *tour[], int nodecount);


int main(int argc, char *argv[]) {
    int nodecount;
    scanf("%d", &nodecount);

    float x[nodecount];
    float y[nodecount];
    int i;
    int j;
    // Read coordinates from stdin
    for(i = 0; i < nodecount; ++i) {
        scanf("%f %f", &x[i], &y[i]);
    }

    //print_farray(x, nodecount);
    //print_farray(y, nodecount);

    int distances_size = nodecount*(nodecount-1)/2;
    int distances[distances_size];

    // Calculate pairwise distances
    for(i = 1; i < nodecount; ++i) {
        for(j = 0; j < i; ++j) {
            distances[get_index(i,j)] = distance(x[i], y[i], x[j], y[j]);
        }
    }
    //print_iarray(distances, distances_size);
    //print_diag_matrix(distances, nodecount);

    int tour[nodecount];
    int tourlength = tsp(distances, tour, nodecount);

    //fprintf(stdout,"Tourlength: %d\n", tourlength);
    print_tour(tour, nodecount);

    return 0;
}

int tsp(int distances[], int tour[], int nodecount) {
    int used[nodecount];
    int best;
    int i, j;
    int tourlength = 0;
    tour[0] = 0;
    used[0] = 1;
    // Initialize used (visited) array
    for (i = 1; i < nodecount; ++i) {
        used[i] = 0;
    }
    // Nearest neighbour
    for(i = 1; i < nodecount; ++i) {
        best = -1;
        int d = 0;
        int bestDistance = 10e7;
        for(j = 0; j < nodecount; ++j) {
            if(used[j] == 0) {
                d = distances[get_index(tour[i-1],j)];
                if (best == -1 || d < bestDistance) {
                   best = j;
                   bestDistance = d;
                }
            }
        }
        tour[i] = best;
        tourlength += bestDistance;
        //fprintf(stdout,"Added to tourlength: %d\n", bestDistance);
        used[best] = 1;
    }
    two_opt(distances, &tour, nodecount);
    return tourlength;
}

void two_opt(int distances[], int *tour[], int nodecount) {
    // TODO: Implement 2-opt
    int i;
    int j;
    int k;
    int m;
    int temp1;
    for(i = 0; i < nodecount-2; ++i) {
        for(j = nodecount-1; j > i+2; --j) {
            if(distances[get_index((*tour)[i],(*tour)[j-1])] + distances[get_index((*tour)[j],(*tour)[i+1])] <
                    distances[get_index((*tour)[i],(*tour)[i+1])] + distances[get_index((*tour)[j-1],(*tour)[j])]) {
                //printf("Swap %d-%d and %d-%d\n", i, i+1, j-1, j);
                //printf("distances %d+%d < %d+%d\n", distances[get_index((*tour)[i],(*tour)[j-1])], distances[get_index((*tour)[j],(*tour)[i+1])], distances[get_index((*tour)[i],(*tour)[i+1])], distances[get_index((*tour)[j-1],(*tour)[j])]);
                m = j-1;
                for(k = i+1; k < m; k++) {
                    temp1 = (*tour)[k];
                    (*tour)[k] = (*tour)[m];
                    (*tour)[m] = temp1;
                    --m;
                }
                //print_tour(*tour, nodecount);
            }
        }
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
    // return floor(sqrt(pow(x2-x1, 2) + pow(y2-y1,2)) + 0.5);
    return nearbyintf(sqrt(pow(x2-x1,2) + pow(y2-y1,2)));
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
