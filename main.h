#ifndef TRAVELGUIDE_MAIN_H
#define TRAVELGUIDE_MAIN_H

void print_farray(float array[], int length);
void print_iarray(int array[], int length);
void print_sarray(short array[], int length);
void print_tour(short array[]);
void print_diag_matrix(int matrix[], int nodecount);
typedef unsigned long bitfield_t[1024/sizeof(long)];

#endif
