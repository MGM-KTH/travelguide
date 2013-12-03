#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

#define NBURS 5
#define RAND

/* Prototypes */
static int distance(float x1, float y1, float x2, float y2);
static int get_index(int a, int b);
void tsp(int dist[], short tour[], int N);
void two_opt(int dist[], short tour[], int N);
void two_point_five_opt(int dist[], short tour[], int N);
void flip_cities(short tour[], int i, int j, int N);
void move_city(short tour[], int N, int b, int e, int i, int j);


int main(int argc, char *argv[]) {
    srand(time(NULL));
    int N;
    scanf("%d", &N);

    float x[N];
    float y[N];
    int i, j, k;
    // Read coordinates from stdin
    for(i = 0; i < N; ++i) {
        scanf("%f %f", &x[i], &y[i]);
    }

    //print_farray(x, N);
    //print_farray(y, N);

    int dist_size = N*(N-1)/2;
    int dist[dist_size];

    short **neighbours = malloc(sizeof(short)*N*NBURS);
    for(i = 0; i < NBURS; ++i) {
        neighbours[0][i] = -1;
    }

    // Calculate pairwise dist
    for(i = 1; i < N; ++i) {

        // Piggyback neighbour initialization
        for(j = 0; j < NBURS; ++j) {
            neighbours[i][j] = -1;
        }

        for(j = 0; j < i; ++j) {
            dist[get_index(i,j)] = distance(x[i], y[i], x[j], y[j]);
        }
    }

    // Calculate closest neighbours for every node. Lowest index is closest.
    int d, node, tmp;
    for(i = 0; i < N; ++i) {
        for(j = 0; j < N; ++j) {
            if(i != j) { // Don't check against self
                node = j;
                d = dist[get_index(i, j)];
                for(k = 0; k < NBURS; ++k) {
                    if(neighbours[i][k]==-1) { // Spot is empty, use and break
                        neighbours[i][k] = node;
                        break;
                    }else if(d < dist[get_index(i,neighbours[i][k])]) {
                        // Closer neighbour found! replace old naeighbour and try to move old neighbour to next spot
                        tmp = neighbours[i][k];
                        d = dist[get_index(i,neighbours[i][k])];
                        neighbours[i][k] = node;
                        node = tmp;
                    }
                }
            }
        }
    }

    /*
    for(i = 0; i < N; ++i) {
        printf("%d ", i);
        print_sarray(neighbours[i], NBURS);
    }
    */
    //print_diag_matrix(dist, N);

    short tour[N];
    tsp(dist, tour, N);
    print_tour(tour, N);

    free(neighbours);
    return 0;
}

void tsp(int dist[], short tour[], int N) {
    short used[N];
    short best;
    int i, j, k;
    tour[0] = 0;
    used[0] = 1;
    // Initialize used (visited) array
    for (i = 1; i < N; ++i) {
        used[i] = 0;
    }
    // Nearest neighbour
    for(i = 1; i < N; ++i) {
        best = -1;
        int d = 0;
        int bestDistance = 10e7;
        for(j = 0; j < N; ++j) {
            if(used[j] == 0) {
                d = dist[get_index(tour[i-1],j)];
                if (best == -1 || d < bestDistance) {
                   best = (short) j;
                   bestDistance = d;
                }
            }
        }
        tour[i] = best;
        used[best] = 1;
    }
    for(k = 0; k < 5; ++k) {
        two_opt(dist, tour, N);
    }
    two_point_five_opt(dist, tour, N);
}

void two_opt(int dist[], short tour[], int N) {
    // TODO: Implement 2-opt
    int i, j;

#ifdef RAND
    int r = rand() % N;
#else
    int r = 0;
#endif

    for(i = r; i < N-2+r; ++i) {
        for(j = N-1+r; j > i+2; --j) {
            if(dist[get_index(tour[i%N],tour[(j-1)%N])] + dist[get_index(tour[j%N],tour[(i+1)%N])] <
                    dist[get_index(tour[i%N],tour[(i+1)%N])] + dist[get_index(tour[(j-1)%N],tour[j%N])]) {
                //printf("Swap %d-%d and %d-%d\n", i, i+1, j-1, j);
                //printf("dist %d+%d < %d+%d\n", dist[get_index(tour[i],tour[j-1])], dist[get_index(tour[j],tour[i+1])], dist[get_index(tour[i],tour[i+1])], dist[get_index(tour[j-1],tour[j])]);

                flip_cities(tour,i,j,N);
            }
        }
    }
}

void flip_cities(short tour[], int i, int j, int N) {
    int k, m;
    short temp1;
    m = j-1;
    for(k = i+1; k < m; k++) {
        temp1 = tour[k%N];
        tour[k%N] = tour[m%N];
        tour[m%N] = temp1;
        --m;
    }
}

/* 2.5-opt */
void two_point_five_opt(int dist[], short tour[], int N) {
    int i, j, a, b, c, d, e;
    int improvement = 1;
    int iters = 0;
    while(improvement && iters <= 2) {
        improvement = 0;
        for(i = 0; i < (N-2); ++i) {
            a = tour[i];
            b = tour[i+1];
            c = tour[i+2];
            for (j = 0; j < (N-1); ++j) {
                if (abs(i-j) <= 2)
                    continue;
                d = tour[j];
                e = tour[j+1];
                // fprintf(stdout, "a: %d, b: %d, c: %d, d: %d, e: %d, i: %d, j: %d\n", a,b,c,d,e, i, j);
                if ((dist[get_index(a, b)] + dist[get_index(b, c)] + dist[get_index(d,e)]) > (dist[get_index(a,c)] + dist[get_index(d,b)] + dist[get_index(b,e)])) {
                    move_city(tour,N,b,e,i,j);
                    improvement = 1;
                    i = 0;    // restart outer for-loop
                    break;
                }
            }
        }
        ++iters;
    }
}

/*
 * Help function for two_point_five_opt.
 * Changes the tour from a-b-c-d-e to a-c-d-b-e,
 * moving b from between a and c to between d and e.
 */
void move_city(short tour[], int N, int b, int e, int i, int j) {
    int m, temp1, temp2;
    if (i < j) {
        // Shift the tour to the left, removing the old b
        for(m = (i+2); m <= j; ++m) {
            tour[m-1] = tour[m];
        }
        // insert new b between j and j+1
        tour[j] = b; 
    }
    else if (i > j) {
        // insert b after d
        temp1 = e;
        tour[j+1] = b;
        // push to the right until old b, replacing it.
        for(m = j+2; m <= (i + 1); ++m) {
            temp2 = tour[m];
            tour[m] = temp1;
            temp1 = temp2;
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

void print_diag_matrix(int matrix[], int N) {
    int i;
    for(i = 1; i < N; ++i) {
        print_iarray(&matrix[get_index(i, 0)], i);
    }
}

int distance(float x1, float y1, float x2, float y2) {
    // return floor(sqrt(pow(x2-x1, 2) + pow(y2-y1,2)) + 0.5);
    return nearbyintf(sqrt(pow(x2-x1,2) + pow(y2-y1,2)));
}

int get_index(int a, int b) {
    // We do not store the trivial 0-distance x --> x
    assert(a != b);
    // We only store the distance in one direction, a --> b since it's
    // Euclidean dist. i.e. a --> b = b --> a.
    // Swap a and b if b > a.
    if (b > a) {
        int temp = a;
        a = b;
        b = temp;
    }
    // Calculate the array index using the formula for summing the first x integers.
    return a*(a-1)/2+b;
}
