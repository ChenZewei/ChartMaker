#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <unistd.h>
#include <dirent.h>
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

void get_logs(string path, vector<string>& files);

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

	//getFiles(path, dir);

	vector<string> files;
	//read_line(path + dir, files);
	get_logs(path, files);
/*
	foreach(files, file)
		cout<<*file<<endl;
*/
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

	chart.SetGraphSize(1000, 600);
	chart.SetGraphQual(3);
	chart.AddData(srs);

	chart.ExportLineChart("output/result", "", u_range.min, u_range.max, step, PNG|EPS|SVG);
	string file_name = "output/result-logs.csv";
	//chart.ExportLineChart("result", "", u_range.min, u_range.max, step, PNG|EPS);
	//string file_name = "result-logs-new.csv";
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

void get_logs(string basePath, vector<string>& files)
{
	DIR *dir;
    struct dirent *ptr;
    string base;

    if ((dir=opendir(basePath.data())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
    while ((ptr=readdir(dir)) != NULL)
    {
		///current dir OR parrent dir
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
            continue;
        else if(ptr->d_type == 8)    ///file
		{
			string path = basePath;
			string buf;
			SchedResultSet srs;
			if(0 == strcmp(ptr->d_name,"result-logs.csv"))
			{
				path += "/";
				path += ptr->d_name;
				files.push_back(path);
//				cout<<"input file:"<<path<<endl;
			}
		}
        else if(ptr->d_type == 4)    ///dir
        {
/*
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
*/
			base = basePath;
			base += "/";
			base += ptr->d_name;
            //readFileList(base, test_attributes);
			get_logs(base, files);
        }
    }

	closedir(dir);
}
