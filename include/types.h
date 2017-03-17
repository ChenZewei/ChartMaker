#ifndef TYPES_H
#define TYPES_H

#include <string.h>
//including GNU Multiple Precision Arithmetic Library
#include <gmpxx.h>
#include <vector>
#include <list>
#include <set>
#include <math.h>
#include <time.h>
#include "iteration-helper.h"

#define uint unsigned int
#define ulong unsigned long

#define MAX_LONG 0xffffffffffffffff
#define MAX_INT  0xffffffff
#define _EPS 0.000001

//output file format
#define PNG 	0x01
#define EPS 	0x02
#define SVG 	0x04
#define TGA 	0x08
#define JSON 	0x10

using namespace std;

typedef mpz_class int_t;		//multiple precision integer
typedef mpq_class fraction_t;		//multiple precision rational number
typedef mpf_class floating_t;		//multiple precision float number

typedef struct
{
	double	utilization;
	uint 	exp_time;
	uint	success_time;
}Result;


#endif
