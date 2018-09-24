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
#include <types.h>
#include <param.h>
#include <sched_result.h>
#include <data_extractor.h>
#include <mgl_chart.h>
#include <xml.h>

using namespace std;

static uint exp_t = 0;
static uint exp_c = 0;

static double step;
static Range u_range;

void getFiles(string path, string dir);

void get_logs(string path, vector<string>& files);

vector<Param> get_parameters();

int main(int argc, char** argv)
{
	XML::LoadFile("config.xml");

	Test_Attribute_Set test_attributes;
	Double_Set steps;
	Range_Set u_ranges;
	XML::get_method(&test_attributes);
	XML::get_utilization_range(&u_ranges);
	XML::get_step(&steps);

	vector<Param> parameters = get_parameters();

	Param parameter = parameters[0];

	// step = steps[0];
	// u_range = u_ranges[0];
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

	chart.SetGraphSize(parameter.graph_width, parameter.graph_height);
	chart.SetGraphQual(parameter.graph_quality);
	chart.AddData(srs);

	// chart.ExportLineChart("output/result", "", u_range.min, u_range.max, step, PNG|EPS|SVG);
	chart.ExportLineChart("output/result", parameter.graph_title, parameter.reserve_range_1.min, parameter.reserve_range_1.max,
                       parameter.reserve_double_1, PNG|EPS|SVG, parameter.graph_x_label, parameter.graph_y_label, parameter.graph_legend_pos);
	string file_name = "output/result-logs.csv";
	//chart.ExportLineChart("result", "", u_range.min, u_range.max, step, PNG|EPS);
	//string file_name = "result-logs-new.csv";
	ofstream output_file(file_name, ofstream::app);
	double utilization = parameter.reserve_range_1.min;
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
		utilization += parameter.reserve_double_1;
	}
	while(utilization < parameter.reserve_range_1.max || fabs(parameter.reserve_range_1.max - utilization) < _EPS);
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

vector<Param> get_parameters() {
  vector<Param> parameters;

  XML::LoadFile("config.xml");

  if (0 == access(string("results").data(), 0)) {
    printf("results folder exsists.\n");
  } else {
    printf("results folder does not exsist.\n");
    if (0 == mkdir(string("results").data(), S_IRWXU))
      printf("results folder has been created.\n");
    else
      return parameters;
  }

  // Server info
  const char* ip = XML::get_server_ip();
  uint port = XML::get_server_port();

  Int_Set lambdas, p_nums;
  Double_Set steps, ratio;
  Range_Set p_ranges, u_ranges, d_ranges;
  Test_Attribute_Set test_attributes;
  uint exp_times;

  // scheduling parameter
  XML::get_method(&test_attributes);
  exp_times = XML::get_experiment_times();
  XML::get_lambda(&lambdas);
  XML::get_processor_num(&p_nums);
  XML::get_period_range(&p_ranges);
  XML::get_deadline_propotion(&d_ranges);
  XML::get_utilization_range(&u_ranges);
  XML::get_step(&steps);
  XML::get_doubles(&ratio, "processor_ratio");

  // resource parameter
  Int_Set resource_nums, rrns, mcsns;
  Double_Set rrps, tlfs;
  Range_Set rrrs;
  XML::get_resource_num(&resource_nums);
  XML::get_resource_request_probability(&rrps);
  XML::get_resource_request_num(&rrns);
  XML::get_resource_request_range(&rrrs);
  XML::get_total_len_factor(&tlfs);
  XML::get_integers(&mcsns, "mcsn");

  // graph parameters
  Range_Set job_num_ranges;
  Range_Set arc_num_ranges;
  Int_Set is_cyclics;
  Int_Set max_indegrees;
  Int_Set max_outdegrees;
  Double_Set para_probs, cond_probs, arc_densities;
  Int_Set max_para_jobs, max_cond_branches;

  XML::get_ranges(&job_num_ranges, "dag_job_num_range");
  XML::get_ranges(&arc_num_ranges, "dag_arc_num_range");
  XML::get_integers(&is_cyclics, "is_cyclic");
  XML::get_integers(&max_indegrees, "max_indegree");
  XML::get_integers(&max_outdegrees, "max_outdegree");
  XML::get_doubles(&para_probs, "paralleled_probability");
  XML::get_doubles(&cond_probs, "conditional_probability");
  XML::get_doubles(&arc_densities, "dag_arc_density");
  XML::get_integers(&max_para_jobs, "max_paralleled_job");
  XML::get_integers(&max_cond_branches, "max_conditional_branch");

  // reserved parameters
  Int_Set reserve_ints_1;
  Int_Set reserve_ints_2;
  Int_Set reserve_ints_3;
  Int_Set reserve_ints_4;
  Double_Set reserve_doubles_1;
  Double_Set reserve_doubles_2;
  Double_Set reserve_doubles_3;
  Double_Set reserve_doubles_4;
  Range_Set reserve_ranges_1;
  Range_Set reserve_ranges_2;

  XML::get_integers(&reserve_ints_1, "reserve_int_1");
  XML::get_integers(&reserve_ints_2, "reserve_int_2");
  XML::get_integers(&reserve_ints_3, "reserve_int_3");
  XML::get_integers(&reserve_ints_4, "reserve_int_4");
  XML::get_doubles(&reserve_doubles_1, "reserve_double_1");
  XML::get_doubles(&reserve_doubles_2, "reserve_double_2");
  XML::get_doubles(&reserve_doubles_3, "reserve_double_3");
  XML::get_doubles(&reserve_doubles_4, "reserve_double_4");
  XML::get_ranges(&reserve_ranges_1, "reserve_range_1");
  XML::get_ranges(&reserve_ranges_2, "reserve_range_2");

  foreach(lambdas, lambda)
  foreach(p_nums, p_num)
  foreach(steps, step)
  foreach(p_ranges, p_range)
  foreach(u_ranges, u_range)
  foreach(d_ranges, d_range)
  foreach(resource_nums, resource_num)
  foreach(rrns, rrn)
  foreach(mcsns, mcsn)
  foreach(rrps, rrp)
  foreach(tlfs, tlf)
  foreach(rrrs, rrr)
  foreach(job_num_ranges, job_num_range)
  foreach(arc_num_ranges, arc_num_range)
  foreach(max_indegrees, max_indegree)
  foreach(max_outdegrees, max_outdegree)
  foreach(para_probs, para_prob)
  foreach(cond_probs, cond_prob)
  foreach(arc_densities, arc_density)
  foreach(max_para_jobs, max_para_job)
  foreach(max_cond_branches, max_cond_branch)
  foreach(reserve_ints_1, reserve_int_1)
  foreach(reserve_ints_2, reserve_int_2)
  foreach(reserve_ints_3, reserve_int_3)
  foreach(reserve_ints_4, reserve_int_4)
  foreach(reserve_doubles_1, reserve_double_1)
  foreach(reserve_doubles_2, reserve_double_2)
  foreach(reserve_doubles_3, reserve_double_3)
  foreach(reserve_doubles_4, reserve_double_4)
  foreach(reserve_ranges_1, reserve_range_1)
  foreach(reserve_ranges_2, reserve_range_2) {
    Param param;
    // set parameters
    param.id = parameters.size();
    param.server_ip = ip;
    param.port = port;
    param.lambda = *lambda;
    param.p_num = *p_num;
    param.ratio = ratio;
    param.step = *step;
    param.p_range = *p_range;
    param.u_range = *u_range;
    param.d_range = *d_range;
    param.test_attributes =
        test_attributes;
    param.exp_times = exp_times;
    param.resource_num = *resource_num;
    param.mcsn = *mcsn;
    param.rrn = *rrn;
    param.rrp = *rrp;
    param.tlf = *tlf;
    param.rrr = *rrr;

    param.job_num_range =
        *job_num_range;
    param.arc_num_range =
        *arc_num_range;

    if (0 == is_cyclics[0])
      param.is_cyclic = false;
    else
      param.is_cyclic = true;

    param.max_indegree = *max_indegree;
    param.max_outdegree =
        *max_outdegree;
    param.para_prob = *para_prob;
    param.cond_prob = *cond_prob;
    param.arc_density = *arc_density;
    param.max_para_job = *max_para_job;
    param.max_cond_branch =
        *max_cond_branch;

    param.graph_width = XML::get_integer("graph_width");
    param.graph_height = XML::get_integer("graph_height");
    param.graph_quality = XML::get_integer("graph_quality");
    param.graph_legend_pos = XML::get_integer("graph_legend_pos");
    param.graph_title = XML::get_string("graph_title");
    param.graph_x_label = XML::get_string("graph_x_label");
    param.graph_y_label = XML::get_string("graph_y_label");

    param.reserve_int_1 = *reserve_int_1;
    param.reserve_int_2 = *reserve_int_2;
    param.reserve_int_3 = *reserve_int_3;
    param.reserve_int_4 = *reserve_int_4;
    param.reserve_double_1 = *reserve_double_1;
    param.reserve_double_2 = *reserve_double_2;
    param.reserve_double_3 = *reserve_double_3;
    param.reserve_double_4 = *reserve_double_4;
    param.reserve_range_1 = *reserve_range_1;
    param.reserve_range_2 = *reserve_range_2;

    parameters.push_back(param);
  }
  cout << "param num:" << parameters.size() << endl;
  return parameters;
}
