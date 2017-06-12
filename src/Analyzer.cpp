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

int readFileList(char *basePath, Test_Attribute_Set& test_attributes);

int main()
{
	DIR *dir;
    char basePath[1000];
	XML::LoadFile("config.xml");

	Test_Attribute_Set test_attributes;
	Double_Set steps;
	Range_Set u_ranges;
	XML::get_method(&test_attributes);
	XML::get_utilization_range(&u_ranges);
	XML::get_step(&steps);

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
	cout<<"Not worse EXP:"<<exp_c<<endl;
	cout<<"Ratio:"<<ratio.get_d()<<endl;
    return 0;
}

int readFileList(char *basePath, Test_Attribute_Set& test_attributes)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(basePath)) == NULL)
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
					log_extract_by_line(srs, buf, test_attributes);
				}

				input_file.close();

//cout<<""<<endl;


				vector<string> targets, others;

				foreach(test_attributes, ta)
				{
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

/*
				foreach(targets, test)
				{
					cout<<"Target:"<<*test<<endl;
				}

				foreach(others, test)
				{
					cout<<"Others:"<<*test<<endl;
				}
*/

				SchedResult& obj = srs.get_sched_result(test_attributes[0].rename);
				bool not_worse;

				double utilization = u_range.min;

				do
				{
//cout<<"Utilization:"<<utilization<<endl;
					not_worse = false;
					vector<fraction_t> target_ratios;
					foreach(targets, test)
					{
						SchedResult& obj = srs.get_sched_result(*test);
						Result temp = obj.get_result_by_utilization(utilization);
						if(0 == temp.exp_num)
							continue;
						
						fraction_t ratio = temp.success_num;
						ratio /= temp.exp_num;

						target_ratios.push_back(ratio);
					}

					foreach(others, test)
					{
						SchedResult& obj = srs.get_sched_result(*test);
						Result temp = obj.get_result_by_utilization(utilization);
						
						if(0 == temp.exp_num)
							continue;
						
						fraction_t ratio = temp.success_num;
						ratio /= temp.exp_num;

						foreach(target_ratios, target_ratio)
						{
							if(ratio <= *target_ratio)
							{	
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
				if(not_worse)
				{
					exp_c++;
					cout<<path<<endl;
				}
			}
			
		}
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base, test_attributes);
        }
    }

	
    closedir(dir);
    return 1;
}
