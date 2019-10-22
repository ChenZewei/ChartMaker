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
#include "param.h"
#include "mgl_chart.h"
#include "xml.h"

using namespace std;

int readFileList(string basePath);

int main()
{
	DIR *dir;
    char basePath[1000];

  ///get the current absoulte path
  memset(basePath,'\0',sizeof(basePath));
  getcwd(basePath, 999);

  ///get the file list
  memset(basePath,'\0',sizeof(basePath));
  strcpy(basePath,"./input");
  readFileList(basePath);
  return 0;
}

int readFileList(string basePath)
{
    DIR *dir;
    struct dirent *ptr;
    string base;

    if ((dir=opendir(basePath.data())) == NULL) {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL) {
		///current dir OR parrent dir
      if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
          continue;
      else if(ptr->d_type == 8) {  // file
        string path = basePath;
        string buf;
        SchedResultSet srs;
        if(0 == strcmp(ptr->d_name,"result-logs.csv")) {
          vector<Param> parameters = get_parameters(basePath+"/config.xml");
          Param parameter = parameters[0];

          path += "/";
          path += ptr->d_name;

          ifstream input_file(path.data(), ifstream::in);

          while(getline(input_file, buf))
          {
            log_extract_by_line(srs, buf, parameter.test_attributes);
          }
          input_file.close();

          string output_file_name = basePath + "/result-logs-compressed.csv";
          ofstream output_file(output_file_name, ofstream::app);
          double x = parameter.reserve_range_1.min;
          do
          {
            foreach(srs.get_sched_result_set(), sr)
            {
              stringstream buf;
              Result result = sr->get_result_by_x(x);
              buf<<sr->get_test_name()<<"\t"<<x<<"\t"<<result.exp_num<<"\t"<<result.success_num;
              output_file<<buf.str()<<"\n";
              output_file.flush();
            }
            x += parameter.reserve_double_1;
          }
          while(x < parameter.reserve_range_1.max || fabs(parameter.reserve_range_1.max - x) < _EPS);
          output_file.close();
        }
		  } else if(ptr->d_type == 4) {  // dir
						base = basePath;
						base += "/";
						base += ptr->d_name;
            readFileList(base);
    }
    }
    closedir(dir);
    return 1;
}
