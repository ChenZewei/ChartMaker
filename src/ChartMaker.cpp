#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <unistd.h>
//#include <gtkmm.h>
//#include "mgl2/mgl.h"
#include "types.h"
#include "sched_result.h"
#include "data_extractor.h"
#include "mgl_chart.h"
#include "xml.h"

using namespace std;

static uint exp_t = 0;
static uint exp_c = 0;

static double step;
static Range u_range;

void getFiles(string path, string dir);

int main(int argc, char** argv)
{
	XML::LoadFile("config.xml");

	Test_Attribute_Set test_attributes;
	Double_Set steps;
	Range_Set u_ranges;
	XML::get_method(&test_attributes);
	XML::get_utilization_range(&u_ranges);
	XML::get_step(&steps);

	step = steps[0];
	u_range = u_ranges[0];

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
//			cout<<buf<<endl;
			log_extract_by_line(srs, buf, test_attributes);
		}

		input_file.close();
	}
	
	chart.SetGraphSize(1280, 720);
	chart.SetGraphQual(3);
	chart.AddData(srs);

	chart.ExportLineChart("output/result", "", u_range.min, u_range.max, step, PNG);

	string file_name = "output/result-logs.csv";
	ofstream output_file(file_name, ofstream::app);
	double utilization = u_range.min;
	do
	{
		foreach(srs.get_sched_result_set(), sr)
		{
			stringstream buf;
			Result result = sr->get_result_by_utilization(utilization);
			buf<<sr->get_test_name()<<"\t"<<utilization<<"\t"<<result.exp_num<<"\t"<<result.success_num;
			output_file<<buf.str()<<"\n";
			output_file.flush();
		}
		utilization += step;
	}
	while(utilization < u_range.max || fabs(u_range.max - utilization) < _EPS);
	output_file.close();

	return 0;
}

void getFiles(string path, string dir)  
{  
	string cmd = "ls " + path + " > " + path + dir;
	system(cmd.data());
}  

