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

bool log_extract_by_line(SchedResultSet& srs, string bufline, Test_Attribute_Set set)
{
	char *charbuf;
	string style;
	bool check = false;
	uint index;
	if(0 < set.size())
		check = true;

	try
	{
//extract test name
	charbuf = strtok(bufline.data(), "\t\n");
	if(NULL == charbuf)
		return false;
//cout<<"test_name:"<<charbuf<<endl;
	string test_name = charbuf;

	if(check)
	{
		bool ignore = true;
		for(uint i = 0; i < set.size(); i++)
		{
			if(0 == strcmp(test_name.data(), set[i].test_name.data()))
			{
				ignore = false;
				style = set[i].style;
			}
			if(0 == strcmp(test_name.data(), set[i].rename.data()))
			{
				ignore = false;
				style = set[i].style;
			}
		}

		if(ignore)
			return false;
	}

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


	SchedResult& sr = srs.get_sched_result(test_name, style);

//cout<<sr.get_test_name()<<":"<<endl;
	sr.insert_result(utilization.get_d(), exp_time.get_d(), success_time.get_d());

	}
	catch(exception &e)
	{
		cout<<"extract exception."<<endl;
	}

	return true;
}
