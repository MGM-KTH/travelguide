#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

#define NBURS 5

/* Prototypes */
static int distance(float x1, float y1, float x2, float y2);
static int get_index(int a, int b);
int tsp(int distances[], short tour[], int nodecount);
void two_opt(int distances[], short tour[], int nodecount);


int main(int argc, char *argv[]) {
    srand(time(NULL));
    int nodecount;
    scanf("%d", &nodecount);

    float x[nodecount];
    float y[nodecount];
    int i, j, k;
    // Read coordinates from stdin
    for(i = 0; i < nodecount; ++i) {
        scanf("%f %f", &x[i], &y[i]);
    }

    //print_farray(x, nodecount);
    //print_farray(y, nodecount);

    int distances_size = nodecount*(nodecount-1)/2;
    int distances[distances_size];

    short neighbours[nodecount][NBURS];
    for(i = 0; i < NBURS; ++i) {
        neighbours[0][i] = -1;
    }

    // Calculate pairwise distances
    for(i = 1; i < nodecount; ++i) {

        // Piggyback neighbour initialization
        for(j = 0; j < NBURS; ++j) {
            neighbours[i][j] = -1;
        }

        for(j = 0; j < i; ++j) {
            distances[get_index(i,j)] = distance(x[i], y[i], x[j], y[j]);
        }
    }

    // Calculate closest neighbours for every node. Lowest index is closest.
    int dist, node, tmp;
    for(i = 0; i < nodecount; ++i) {
        for(j = 0; j < nodecount; ++j) {
            if(i != j) { // Don't check against self
                node = j;
                dist = distances[get_index(i, j)];
                for(k = 0; k < NBURS; ++k) {
                    if(neighbours[i][k]==-1) { // Spot is empty, use and break
                        neighbours[i][k] = node;
                        break;
                    }else if(dist < distances[get_index(i,neighbours[i][k])]) {
                        // Closer neighbour found! replace old naeighbour and try to move old neighbour to next spot
                        tmp = neighbours[i][k];
                        dist = distances[get_index(i,neighbours[i][k])];
                        neighbours[i][k] = node;
                        node = tmp;
                    }
                }
            }
        }
    }

    /*
    for(i = 0; i < nodecount; ++i) {
        printf("%d ", i);
        print_sarray(neighbours[i], NBURS);
    }
    */
    //print_diag_matrix(distances, nodecount);

    short tour[nodecount];
    int tourlength = tsp(distances, tour, nodecount);

    //fprintf(stdout,"Tourlength: %d\n", tourlength);
    print_tour(tour, nodecount);

    return 0;
}

int tsp(int distances[], short tour[], int nodecount) {
    short used[nodecount];
    short best;
    int i, j, k;
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
                   best = (short) j;
                   bestDistance = d;
                }
            }
        }
        tour[i] = best;
        tourlength += bestDistance;
        //fprintf(stdout,"Added to tourlength: %d\n", bestDistance);
        used[best] = 1;
    }
    for(k = 0; k < 7; ++k) {
        two_opt(distances, tour, nodecount);
    }
    return tourlength;
}

void two_opt(int distances[], short tour[], int nodecount) {
    // TODO: Implement 2-opt
    int i, j, k, m;
    int r = rand() % nodecount;
    short temp1;
    for(i = r; i < nodecount-2+r; ++i) {
        for(j = nodecount-1+r; j > i+2; --j) {
            if(distances[get_index(tour[i%nodecount],tour[(j-1)%nodecount])] + distances[get_index(tour[j%nodecount],tour[(i+1)%nodecount])] <
                    distances[get_index(tour[i%nodecount],tour[(i+1)%nodecount])] + distances[get_index(tour[(j-1)%nodecount],tour[j%nodecount])]) {
                //printf("Swap %d-%d and %d-%d\n", i, i+1, j-1, j);
                //printf("distances %d+%d < %d+%d\n", distances[get_index(tour[i],tour[j-1])], distances[get_index(tour[j],tour[i+1])], distances[get_index(tour[i],tour[i+1])], distances[get_index(tour[j-1],tour[j])]);
                m = j-1;
                // TODO: Possibly fix a data structure that manages this (e.g. Satellite list)
                for(k = i+1; k < m; k++) {
                    temp1 = tour[k%nodecount];
                    tour[k%nodecount] = tour[m%nodecount];
                    tour[m%nodecount] = temp1;
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

void print_sarray(short array[], int length) {
    int i;
    printf("[%d", array[0]);
    for(i = 1; i < length; ++i) {
        printf(", %d", array[i]);
    }
    printf("]\n");
}

void print_tour(short array[], int length) {
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
