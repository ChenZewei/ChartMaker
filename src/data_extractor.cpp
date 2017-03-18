#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <unistd.h>
#include "types.h"
#include "sched_result.h"

void read_line(string path, vector<string>& files)
{
	string buf;
	ifstream dir(path.data(), ifstream::in);
	getline(dir, buf);
	while(getline(dir, buf))
	{
		files.push_back("input/" + buf);
//cout<<"file name:"<<buf<<endl;
	}
}

bool log_extract_by_line(SchedResultSet& srs, string bufline)
{
	char *charbuf;

	try
	{
//extract test name
	charbuf = strtok(bufline.data(), "\t\n");
	if(NULL == charbuf)
		return false;
//cout<<"test_name:"<<charbuf<<endl;
	string test_name = charbuf;

//extract utilization
	charbuf = strtok(NULL, "\t\n");
	if(NULL == charbuf)
		return false;
//cout<<"utilization:"<<charbuf<<endl;
	floating_t utilization(charbuf);

//extract experiment times
	charbuf = strtok(NULL, "\t\n");
	if(NULL == charbuf)
		return false;
//cout<<"exp_time:"<<charbuf<<endl;
	int_t exp_time(charbuf);

//extract success times
	charbuf = strtok(NULL, "\t\n");
	if(NULL == charbuf)
		return false;
//cout<<"success_time:"<<charbuf<<endl;
	int_t success_time(charbuf);
		
	
	SchedResult& sr = srs.get_sched_result(test_name);	
		
//cout<<sr.get_test_name()<<":"<<endl;
	sr.insert_result(utilization.get_d(), exp_time.get_d(), success_time.get_d());	
	
	}
	catch(exception &e)
	{
		cout<<"extract exception."<<endl;
	}

	return true;
}
