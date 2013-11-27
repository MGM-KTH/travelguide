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
    printf("[%f", array[0]);
    for(i = 1; i < length; i++) {
        printf(", %f", array[i]);
    }
    printf("]\n");
}

int distance(float x1, float y1, float x2, float y2) {
    return floor(sqrt(pow(x1-x2, 2) + pow(y1-y2, 2)) + 0.5);
}

int get_index(int a, int b) {
    return a*(a-1)/2+b;
}
