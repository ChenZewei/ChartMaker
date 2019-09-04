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
static uint exp_b = 0;
static uint exp_c = 0;
static uint exp_e = 0;

static double step;
static Range u_range;

int readFileList(string basePath, Test_Attribute_Set& test_attributes);

int main()
{
	DIR *dir;
    char basePath[1000];
	XML::LoadFile("config.xml");

	Test_Attribute_Set test_attributes;
	Double_Set steps;
	Range_Set u_ranges;
	XML::get_method(&test_attributes);
  	XML::get_ranges(&u_ranges, "reserve_range_1");
	XML::get_doubles(&steps, "reserve_double_1");

	step = steps[0];
	u_range = u_ranges[0];

    ///get the current absoulte path
    memset(basePath,'\0',sizeof(basePath));
    getcwd(basePath, 999);
    //printf("the current dir is : %s\n",basePath);

    ///get the file list
    memset(basePath,'\0',sizeof(basePath));
    strcpy(basePath,"./input");
    readFileList(basePath, test_attributes);

	fraction_t ratio = exp_c;
	ratio /= exp_t;
	cout<<"Total EXP:"<<exp_t<<endl;
	cout<<"Dominant EXP:"<<exp_c<<endl;
	cout<<"Better EXP:"<<exp_b<<endl;
	cout<<"Totally equal EXP:"<<exp_e<<endl;
	cout<<"Ratio:"<<ratio.get_d()<<endl;
    return 0;
}

int readFileList(string basePath, Test_Attribute_Set& test_attributes)
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
				exp_t++;
				path += "/";
				path += ptr->d_name;
//				cout<<"input file:"<<path<<endl;

				ifstream input_file(path.data(), ifstream::in);

				while(getline(input_file, buf))
				{
//					cout<<buf<<endl;
					log_extract_by_line(srs, buf, test_attributes);
				}

				input_file.close();

//cout<<""<<endl;


				vector<string> targets, others;

				foreach(test_attributes, ta)
				{
					cout << ta->rename.data() << " " << ta->cluster << endl;
					if(0 == ta->cluster)
					{
						if(0 == strcmp(ta->rename.data(), ""))
							targets.push_back(ta->test_name);
						else
							targets.push_back(ta->rename);
					}
					else
					{
						if(0 == strcmp(ta->rename.data(), ""))
							others.push_back(ta->test_name);
						else
							others.push_back(ta->rename);
					}
				}


				foreach(targets, test)
				{
					cout<<"Target:"<<*test<<endl;
				}

				foreach(others, test)
				{
					cout<<"Others:"<<*test<<endl;
				}


				SchedResult& obj = srs.get_sched_result(test_attributes[0].rename);
				bool not_worse;
				bool better = true;
				bool dominant = false;
				bool equal = true;

				double utilization = u_range.min;

				do
				{
//cout<<"-------------------Utilization:"<<utilization<<endl;
//					not_worse = false;
//					dominant = false;
					vector<fraction_t> target_ratios;

					foreach(targets, test)
					{
						SchedResult& obj = srs.get_sched_result(*test);
						Result temp = obj.get_result_by_utilization(utilization);
						if(0 == temp.exp_num)
							continue;

						fraction_t ratio = temp.success_num;
						ratio /= temp.exp_num;
//cout<<"TARGET:"<<obj.get_test_name()<<" ratio:"<<ratio.get_d()<<endl;
						target_ratios.push_back(ratio);
					}

					if(0 == target_ratios.size())
					{
						utilization += step;
						continue;
					}

					foreach(others, test)
					{
						not_worse = false;
						//dominant = false;
						SchedResult& obj = srs.get_sched_result(*test);
						Result temp = obj.get_result_by_utilization(utilization);

						if(0 == temp.exp_num)
							continue;

						fraction_t ratio = temp.success_num;
						ratio /= temp.exp_num;

//cout<<obj.get_test_name()<<" ratio:"<<ratio.get_d()<<endl;
						foreach(target_ratios, target_ratio)
						{
							if(ratio != *target_ratio)
								equal = false;
							if(ratio < *target_ratio)
								dominant = true;
							if(ratio <= *target_ratio)
							{
//cout<<"Not worse!!!!!!"<<endl;
								not_worse = true;
								break;
							}
						}
						if(!not_worse)
							break;
					}
					if(!not_worse)
						break;

				utilization += step;
				}
				while(utilization < u_range.max || fabs(u_range.max - utilization) < _EPS);

				vector<uint> targets_success;
				vector<uint> others_success;
				foreach(targets, test)
					targets_success.push_back(0);
				foreach(others, test)
					others_success.push_back(0);

				utilization = u_range.min;
				do
				{
					/* code */
					uint i = 0;
					foreach(targets, test)
					{
						SchedResult& obj = srs.get_sched_result(*test);
						Result temp = obj.get_result_by_utilization(utilization);
						targets_success[i++] += temp.success_num;
					}

					uint j = 0;
					foreach(others, test)
					{
						SchedResult& obj = srs.get_sched_result(*test);
						Result temp = obj.get_result_by_utilization(utilization);
						others_success[j++] += temp.success_num;
					}
					utilization += step;
				}
				while(utilization < u_range.max || fabs(u_range.max - utilization) < _EPS);

				if(not_worse && dominant)
				{
					exp_c++;
					cout<<path<<endl;
				}
				else
					cout<<"Not dominant:"<<path<<endl;

				if(equal)
					exp_e++;

				// OR
				uint ts_size = targets_success.size();
				foreach(targets_success, ts)
				{
					// uint os_size = others_sucess.size();
					bool temp_better = true;
					foreach(others_success, os)
					{
						if(*ts<=*os)
						{
							// better = false;
							// break;
							// os_size--;
							temp_better = false;
							break;
						}
					}
					// if(!better)
					// 	break;
					if(!temp_better)
					{
						ts_size--;
					}
				}
				if(0 == ts_size)
					better = false;

				// AND
				// foreach(targets_success, ts)
				// {
				// 	foreach(others_success, os)
				// 	{
				// 		if(*ts<=*os)
				// 		{
				// 			better = false;
				// 			break;
				// 		}
				// 	}
				// 	if(!better)
				// 		break;
				// }


				// foreach(targets_success, ts)
				// 	cout<<"TS:"<<*ts<<endl;
				// foreach(others_success, os)
				// 			cout<<"OS:"<<*os<<endl;
				if(better)
					exp_b++;
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
            readFileList(base, test_attributes);
        }
    }


    closedir(dir);
    return 1;
}
