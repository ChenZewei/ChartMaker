#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <unistd.h>
#include "types.h"
#include "sched_result.h"
#include "mgl_chart.h"

using namespace std;

int main(int argc, char** argv)
{
	string path;
	string buf;
	if(1 == argc)
		path = "input/result-logs.csv";
	else
		path = argv[1];

	if(0 != access(path.data(), 0))
	{
		cout<<path<<" does not exist."<<endl;
		return 1;
	}

	ifstream input_file(path.data(), ifstream::in);
	stringstream ss;

	while(getline(input_file, buf))
	{
		ss<<buf<<"\t";
		string test_name, utilization, e_time, s_time;
		get(ss, test_name, '\t');
		get(ss, utilization, '\t');
		get(ss, e_time, '\t');
		get(ss, s_time, '\t');
		
		cout<<test_name<<"\t"<<utilization<<"\t"<<e_time<<"\t"<<s_time;
	}


	input_file.close();
	return 0;
}
