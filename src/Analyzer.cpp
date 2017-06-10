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

int readFileList(char *basePath, Test_Attribute_Set& test_attributes);

int main()
{
	DIR *dir;
    char basePath[1000];
	XML::LoadFile("config.xml");

	Test_Attribute_Set test_attributes;
	XML::get_method(&test_attributes);

    ///get the current absoulte path
    memset(basePath,'\0',sizeof(basePath));
    getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);

    ///get the file list
    memset(basePath,'\0',sizeof(basePath));
    strcpy(basePath,"./input");
    readFileList(basePath, test_attributes);
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
				path += "/";
				path += ptr->d_name;
				//cout<<"input file:"<<path<<endl;
				
				ifstream input_file(path.data(), ifstream::in);

				while(getline(input_file, buf))
				{
					//cout<<buf<<endl;
					log_extract_by_line(srs, buf, test_attributes);
				}

				input_file.close();

				string target_name;

				if(0 == strcmp(test_attributes[0].rename.data(), ""))
					target_name = test_attributes[0].test_name;
				else
					target_name = test_attributes[0].rename;

//cout<<"Target:"<<target_name<<endl;

/*
				foreach(srs.get_sched_result_set(), set)
				{
					cout<<set->get_test_name()<<endl;

				}
*/

				SchedResult& obj = srs.get_sched_result(test_attributes[0].rename);
				bool exceed = true;

				foreach(obj.get_results(), result)
				{
					fraction_t ratio = result->success_num;
					ratio /= result->exp_num;
//cout<<result->utilization<<endl;

					foreach(srs.get_sched_result_set(), set)
					{
						if(0 == strcmp(obj.get_test_name().data(), set->get_test_name().data()))
							continue;

						//cout<<"Other:"<<set->get_test_name()<<endl;
						
						foreach(set->get_results(), result_t)
						{
							if(abs(result->utilization - result_t->utilization) < _EPS)
							{
								fraction_t ratio_t = result_t->success_num;
								ratio_t /= result_t->exp_num;

								if(ratio < ratio_t)
								{

//			cout<<"Utilization:"<<result->utilization<<endl;
//			cout<<target_name<<":"<<ratio.get_d()<<endl;
//			cout<<set->get_test_name()<<":"<<ratio_t.get_d()<<endl;
			
									exceed = false;
									break;
								}
							}
						}
						if(!exceed)
							break;
					}
					if(!exceed)
						break;
				}
				if(exceed)
					cout<<path<<endl;
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
