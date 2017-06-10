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

int readFileList(char *basePath);

int main()
{
	DIR *dir;
    char basePath[1000];

    ///get the current absoulte path
    memset(basePath,'\0',sizeof(basePath));
    getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);

    ///get the file list
    memset(basePath,'\0',sizeof(basePath));
    strcpy(basePath,"./input");
    readFileList(basePath);
    return 0;
}

int readFileList(char *basePath)
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
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    ///file
            printf("d_name:%s/%s\n",basePath,ptr->d_name);
        else if(ptr->d_type == 10)    ///link file
            printf("d_name:%s/%s\n",basePath,ptr->d_name);
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base);
        }
    }
    closedir(dir);
    return 1;
}
