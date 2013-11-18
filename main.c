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

    return 0;
}

void print_array(float array[], int length) {
    int i;
    for(i = 0; i < length; i++) {
        printf("%f\n", array[i]);
    }
}
