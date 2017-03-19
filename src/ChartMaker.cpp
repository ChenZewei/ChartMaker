#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <unistd.h>
#include "types.h"
#include "sched_result.h"
#include "data_extractor.h"
#include "mgl_chart.h"

using namespace std;

void getFiles(string path, string dir);

int main(int argc, char** argv)
{
	Chart chart;
	SchedResultSet srs;
	string path;
	string buf;
	string dir = "0";
	if(1 == argc)
		path = "input/";
	else
		path = argv[1];
	
	getFiles(path, dir);

	vector<string> files;
	read_line(path + dir, files);

	for(uint i = 0; i < files.size(); i++)
	{
		ifstream input_file(files[i].data(), ifstream::in);
	
		while(getline(input_file, buf))
		{
			//cout<<buf<<endl;
			log_extract_by_line(srs,buf);
		}

		input_file.close();
/*
		cout<<"=================="<<endl;
		cout<<"add file:"<<files[i]<<endl;

		foreach(srs.get_sched_result_set(), sr)
		{
			foreach(sr->get_results(), result)
			{
				cout<<sr->get_test_name()<<"\t"<<result->utilization<<"\t"<<result->exp_time<<"\t"<<result->success_time<<endl;
			}
		}
*/
	}
	
	chart.SetGraphSize(1280, 720);
	chart.SetGraphQual(3);
	chart.AddData(srs);

	chart.ExportLineChart("output/result", "", 0, 4, 0.1, PNG);

	return 0;
}

void getFiles(string path, string dir)  
{  
	string cmd = "ls " + path + " > " + path + dir;
	system(cmd.data());
}  

