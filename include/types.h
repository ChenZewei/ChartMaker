#ifndef TYPES_H
#define TYPES_H

#include <assert.h>
#include <gmpxx.h>  // including GNU Multiple Precision Arithmetic Library
#include <math.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include "iteration-helper.h"

#define uint uint32_t
#define ulong uint64_t

#define MAX_LONG 0xffffffffffffffff
#define MAX_INT 0xffffffff
#define MAX_ULONG 0xffffffffffffffff
#define MAX_UINT 0xffffffff
#define _EPS 0.000001

//output file format
#define PNG 	0x01
#define EPS 	0x02
#define SVG 	0x04
#define TGA 	0x08
#define JSON 	0x10

using namespace std;
using std::cout;
using std::endl;
using std::list;
using std::set;
using std::string;
using std::vector;


typedef mpz_class int_t;		//multiple precision integer
typedef mpq_class fraction_t;		//multiple precision rational number
typedef mpf_class floating_t;		//multiple precision float number

typedef struct
{
	double x;
	uint exp_num;
	uint success_num;
}Result;

typedef struct
{
	double min;
	double max;
}Range;

typedef struct
{
	bool LP;
	uint test_type;
	uint test_method;
	string sched_method;
	string priority_assignment;
	string locking_protocol;
	string test_name;
	string remark;
	string rename;
	string style;
	uint cluster;
}Test_Attribute;

typedef vector<int> Int_Set;
typedef vector<double> Double_Set;
typedef vector<Range> Range_Set;
typedef vector<Result> Result_Set;
typedef vector<Result_Set> Result_Sets;
typedef vector<Test_Attribute> Test_Attribute_Set;
typedef list<void*> TaskQueue;
typedef list<void*> ResourceQueue;
typedef vector<double> Ratio;

#endif
