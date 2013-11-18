#include "main.h"

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
    for(i = 0; i < length; i++) {
        printf("%f\n", array[i]);
    }
}

int get_index(int a, int b) {
    return a*(a-1)/2+b;
}
