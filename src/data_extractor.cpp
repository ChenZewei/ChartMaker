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
	char *ptr = NULL;
	// char buffer[100] = bufline.data();
	char *buf = const_cast<char*>(bufline.data());
	string style;
	bool check = false;
	uint index;
	if(0 < set.size())
		check = true;

	try
	{
//extract test name
	charbuf = strtok_r(buf, "\t\n", &ptr);
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
	charbuf = strtok_r(NULL, "\t\n", &ptr);
	if(NULL == charbuf)
		return false;
//cout<<"utilization:"<<charbuf<<endl;
	floating_t utilization(charbuf);

//extract experiment times
	charbuf = strtok_r(NULL, "\t\n", &ptr);
	if(NULL == charbuf)
		return false;
//cout<<"exp_time:"<<charbuf<<endl;
	int_t exp_time(charbuf);

//extract success times
	charbuf = strtok_r(NULL, "\t\n", &ptr);
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

void extract_element(vector<floating_t>& elements, string bufline, uint start, uint num, const char seperator)
{
	char *charbuf;
	// char buffer[100] = bufline.data();
	char *buf = const_cast<char*>(bufline.data());
	char *ptr = NULL;
	string cut = " \t\r\n";
	cut += seperator;

	uint count = 0;

	try
	{
		while (NULL != (charbuf = strtok_r(buf, cut.data(), &ptr))) {
			if(count >= start && count < start + num)
				{
					cout<<"element:"<<charbuf<<endl;
					floating_t element(charbuf, 100);
					elements.push_back(element);
				}
				count++;
				buf = NULL;
		}

		// Deprecated. (strtok(...))
		// if(NULL != (charbuf = strtok(bufline.data(), cut.data())))
		// 	do
		// 	{
		// 		if(count >= start && count < start + num)
		// 		{
		// 			cout<<"element:"<<charbuf<<endl;
		// 			floating_t element(charbuf, 100);
		// 			elements.push_back(element);
		// 		}
		// 		count++;
		// 	}
		// 	while(NULL != (charbuf = strtok(NULL, cut.data())));
	}
	catch(exception &e)
	{
		cout<<"extract exception."<<endl;
	}
}



