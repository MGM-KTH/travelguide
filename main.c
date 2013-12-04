#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include "main.h"

#define TWO_POINT_FIVE_OPT_ITERS 50
#define RAND

/* Prototypes */
static int distance(float x1, float y1, float x2, float y2);
static int get_index(int a, int b);
static int get_n_index(int i, int j);
void tsp(short neighbours[], int dist[], short tour[], int N);
short get_nearest(int dist[], short used[], int i, int N);
void two_opt(int dist[], short sat[], int N);
void two_point_five_opt(short neighbours[], int dist[], short sat[], int N);
void flip_section(short sat[], int isat, int jsat);
void move_city(short sat[], int N, int a, int b, int c, int d, int e);

void set_timer();
void oot_handler(int signum);

int OUTOFTIME;
int NBURS;


int main(int argc, char *argv[]) {

    OUTOFTIME = 0;
    signal(SIGALRM, oot_handler); // Set signal handler for SIGALRM
    set_timer();                  // Setup timer

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

    if(N <= 40)
        NBURS = N-1;
    else
        NBURS = 40;

    int neighbour_list_size = N*NBURS;
    short neighbours[neighbour_list_size];
    for(i = 0; i < neighbour_list_size; ++i) {
        neighbours[i] = -1;
    }

    // Calculate pairwise dist
    for(i = 1; i < N; ++i) {

        // Piggyback neighbour initialization
        for(j = 0; j < NBURS; ++j) {
            neighbours[get_n_index(i,j)] = -1;
        }

        for(j = 0; j < i; ++j) {
            dist[get_index(i,j)] = distance(x[i], y[i], x[j], y[j]);
        }
    }

    // Calculate closest neighbours for every node. Lowest index is closest.
    int d, node, tmp, neighbour;
    for(i = 0; i < N; ++i) {
        for(j = 0; j < N; ++j) {
            if(i != j) { // Don't check against self
                node = j;
                d = dist[get_index(i, j)];
                for(k = 0; k < NBURS; ++k) {
                    neighbour = neighbours[get_n_index(i,k)];
                    if(neighbour == -1) { // Spot is empty, use and break
                        neighbours[get_n_index(i,k)] = node;
                        break;
                    }
                    else if(d < dist[get_index(i,neighbour)]) {
                        // Closer neighbour found! replace old naeighbour and try to move old neighbour to next spot
                        tmp = neighbour;
                        d = dist[get_index(i,neighbour)];
                        neighbours[get_n_index(i,k)] = node;
                        node = tmp;
                    }
                }
            }
        }
    }

    //print_diag_matrix(dist, N);

    /*
     * Create a sattelite list double the size of the amount of elements
     *
     *  _0___1___2___3___4_ <--- Nodes
     * |2|9|4|1|6|3|8|5|0|7| <--- Satellites
     *  -------------------
     *
     * Index of a sattelite:             i
     * The node a sattelite belongs to:  i>>1
     * The companion sattelite:          i^1
     */
    short sat[2*N];
    //printf("size of satellite list: %d\n", 2*N);
    tsp(neighbours,dist,sat,N);

    //fprintf(stdout,"Tourlength: %d\n", tourlength);
    //print_sarray(sat, 2*N);
    print_tour(sat, N);
    //print_diag_matrix(dist,N);

    //free(neighbours);
    return 0;
}

void set_timer() {
     struct itimerval timer;
     timer.it_value.tv_sec = 1;       /* 1 second */
     timer.it_value.tv_usec = 800000; /* 0.9 seconds */
     timer.it_interval.tv_sec = 0;
     timer.it_interval.tv_usec = 0; 

     setitimer(ITIMER_REAL, &timer, NULL);
}

void oot_handler(int signum) {
    OUTOFTIME = 1;
}

void tsp(short neighbours[], int dist[], short sat[], int N) {
    short used[N];
    short node, best, start;
    int found;
    int i, j, k;

    // Initialize used (visited) array
    for (i = 0; i < N; ++i) {
        used[i] = 0;
    }

#ifdef RAND
    start = rand() % N;
#else
    start = 0;
#endif
    used[start] = 1;
    node = start;

    for (i = 0; i < N-1; ++i) {
        found = 0;
        for (j = 0; j < NBURS; ++j) {
            // check neighbour list
            best = neighbours[get_n_index(node,j)];
            if (best != node) {
                // fprintf(stdout, "best: %d, node: %d, used: %d\n", best, node, used[best]);
                if(!used[best]) {
                    found = 1;
                    break;
                }
            }
        }
        if(!found) {
            // Backup plan: Check all nodes
            best = get_nearest(dist,used,node,N);
        }
        sat[node*2] = best*2;
        sat[(best*2)^1] = (node*2)^1;
        used[best] = 1;
        // fprintf(stdout, "Using %d\n", best);
        node = best;
    }

    // Close the loop, depends on last node value
    sat[best*2] = start*2;
    sat[(start*2)^1] = (best*2)^1;

    for(k = 0; k < 5; ++k) {
        two_opt(dist, sat, N);
    }
    // while(!OUTOFTIME) {
        two_point_five_opt(neighbours, dist, sat, N);
    // }
}

short get_nearest(int dist[], short used[], int i, int N) {
    short best = -1;
    int j, d;
    int bestDistance = 10e7;
    for(j = 0; j < N; ++j) {
        if(j != i && used[j] == 0) {
            d = dist[get_index(i,j)];
            if(best == -1 || d < bestDistance) {
                best = (short) j;
                bestDistance = d;
            }
        }
    }
    return best;
}

void two_opt(int dist[], short sat[], int N) {
    // TODO: Implement 2-opt
    int i, j, inode, jnode, isat, jsat;

    //print_tour(sat, N);
    //printf("\n");

#ifdef RAND
    int start = rand() % N;
#else
    int start = 0;
#endif
    inode = start;
    isat = start*2;  // Get index of forward flow satellite of first node in tour

    for(i = start; i < N-2+start; ++i) {
        jsat = sat[(start*2)^1]; // Get index of counter flow satellite of last node in tour
        jnode = jsat>>1;         // Get last node in tour

        for(j = N-1+start; j > i+2; --j) {
            int i_next = sat[isat]>>1; // Get next node index from forward flow satellite
            int j_prev = sat[jsat]>>1; // Get prev node index from backward flow satellite
            //printf("i = %d i_next = %d j = %d j_prev = %d\n", inode, i_next, jnode, j_prev);
            //printf("isat = %d jsat = %d\n", isat, jsat);
            int old_dist = dist[get_index(inode,i_next)] + dist[get_index(jnode,j_prev)];

            if(dist[get_index(inode,j_prev)] + dist[get_index(jnode,i_next)] < old_dist) {
                //printf("SWAP!\n");
                //print_sarray(sat, 2*N);
                flip_section(sat,isat,jsat);
                //print_sarray(sat, 2*N);
            }

            jsat = sat[jsat];
            jnode = jsat>>1;
        }
        isat = sat[isat];
        inode = isat>>1;
    }
}

void flip_section(short sat[], int isat, int jsat) {

    // Get satellites for next/prev
    int i_next = sat[isat];
    int j_prev = sat[jsat];

    // Set satellites in i and j
    sat[isat] = j_prev;
    sat[jsat] = i_next;

    // Set satellites in i+1 and j-1
    sat[i_next^1] = jsat^1;
    sat[j_prev^1] = isat^1;
}

/* 2.5-opt */
void two_point_five_opt(short neighbours[], int dist[], short sat[], int N) {
    // _i variables refers to an actual city index. 
    // a-e refers to satellite indexes
    int i, j, a, b, c, d, e, a_i, b_i, c_i, d_i, e_i;
    int improvement = 1;
    int iters = 0;
    while(improvement && iters <= TWO_POINT_FIVE_OPT_ITERS) {
        improvement = 0;
        for(i = 0; i < (N-2); ++i) {
            a = sat[i*2]; // Forward node for i
            b = sat[a];   // Next for a
            c = sat[b];
            a_i = a >> 1;   // Get city indexes
            b_i = b >> 1;
            c_i = c >> 1;
            for (j = 0; j < NBURS; ++j) {
                d = sat[neighbours[get_n_index(i,j)]*2]; // Forward node for j
                e = sat[d];   // Next for d
                d_i = d >> 1;
                e_i = e >> 1;
                if (d_i == a_i || d_i == b_i || d_i == c_i)
                    continue;
                // fprintf(stdout, "a: %d, b: %d, c: %d, d: %d, e: %d, i: %d, j: %d\n", a,b,c,d,e, i, j);
                // fprintf(stdout, "Printing tour:\n");
                // print_tour(sat, N);
                if ((dist[get_index(a_i, b_i)] + dist[get_index(b_i, c_i)] + dist[get_index(d_i,e_i)]) > (dist[get_index(a_i,c_i)] + dist[get_index(d_i,b_i)] + dist[get_index(b_i,e_i)])) {
                    move_city(sat,N,a,b,c,d,e);
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
void move_city(short sat[], int N, int a, int b, int c, int d, int e) {
    sat[d] = b;                // D --> B
    sat[b^1] = d^1;            // B <-- D
    sat[b] = e;                // B --> E
    sat[e^1] = b^1;            // E <-- B
    sat[a] = c;                // A --> C
    sat[c^1] = a^1;            // C <-- A
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
    int i, index, satellite;
    satellite = 0;
    for(i = 0; i < length; ++i) {
        index = satellite>>1;
        satellite = array[satellite];
        printf("%d\n", index);
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

int get_n_index(int i, int j) {
    return i*NBURS + j;
}


