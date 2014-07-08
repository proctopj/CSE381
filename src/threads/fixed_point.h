#define F (1 << 14)

/* Convert int to fixed point */
int int2fixed_point (int n);
/* Convert fixed point to int, rounding */
int fixed_point2int_round (int x);
/* Convert fixed point to int */
int fixed_point2int (int x);
/* Add fixed point and another fixed point */
int fixed_point_add (int x, int y);
/* Add fixed point and an int */
int fixed_point_add_mixed (int x, int n);
/* Subtract fixed point and another fixed point */
int fixed_point_sub (int x, int y);
/* Subtract fixed point and an int */
int fixed_point_sub_mixed (int x, int n);
/* Multiply fixed point and another fixed point */
int fixed_point_multiply (int x, int y);
/* Multiply fixed point and an int */
int fixed_point_multiply_mixed (int x, int n);
/* Divide fixed point and another fixed point */
int fixed_point_divide (int x, int y);
/* Divide fixed point and an int */
int fixed_point_divide_mixed (int x, int n);

#include "fixed_point.c"
