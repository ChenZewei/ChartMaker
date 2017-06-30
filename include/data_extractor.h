#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H

#include <types.h>

class SchedResult;
class SchedResultSet;

void read_line(string path, vector<string>& files);
void log_extract_by_line(SchedResultSet& srs, string bufline, Test_Attribute_Set set);
void extract_element(vector<floating_t>& elements, string bufline, uint start = 0, uint num = MAX_INT, const char seperator = ',');
#endif
