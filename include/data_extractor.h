#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H

#include <types.h>

class SchedResult;
class SchedResultSet;

void read_line(string path, vector<string>& files);
void log_extract_by_line(SchedResultSet& srs, string bufline);

#endif
