
CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_PRINTSUPPORT_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -g -D_REENTRANT -fPIE $(DEFINES) -std=c++11 -std=gnu++11 -fpermissive
GTKCFLAGS	  =	`pkg-config gtkmm-3.0 --cflags`
GTKLIBS	  	  =	`pkg-config gtkmm-3.0 --libs`
INCPATH       = -I. -I../include -I../include/mathGL
LINK          = g++
LFLAGS        = #-Wl,-O1
LIBS          = -lgmp -lgmpxx -lmgl -lpthread -lglpk
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f
MKDIR         = mkdir -p
TARGET	      = ChartMaker
OBJ_FILE	  = sched_result.o data_extractor.o mgl_chart.o xml.o tinyxml2.o random_gen.o param.o  

ChartMaker:ChartMaker.o $(OBJ_FILE)  
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $(TARGET) ChartMaker.o $(OBJ_FILE) $(LFLAGS) $(LIBS)

Analyzer:Analyzer.o $(OBJ_FILE)  
	$(CXX) $(CXXFLAGS) $(INCPATH) -o Analyzer Analyzer.o $(OBJ_FILE)  $(LFLAGS) $(LIBS)

logCompress:logCompress.o $(OBJ_FILE)  
	$(CXX) $(CXXFLAGS) $(INCPATH) -o logCompress logCompress.o $(OBJ_FILE)  $(LFLAGS) $(LIBS)

test:test.o $(OBJ_FILE)  
	$(CXX) $(CXXFLAGS) $(INCPATH) -o test test.o $(OBJ_FILE)  $(LFLAGS) $(LIBS)

ChartMaker.o:ChartMaker.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c ChartMaker.cpp $(LIBS)

Analyzer.o:Analyzer.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c Analyzer.cpp $(LIBS)

logCompress.o:logCompress.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c logCompress.cpp $(LIBS)

test.o:test.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c test.cpp $(LIBS)

sched_result.o:sched_result.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c sched_result.cpp $(LIBS)

sched_result.o:sched_result.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c sched_result.cpp $(LIBS)

data_extractor.o:data_extractor.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c data_extractor.cpp $(LIBS)

mgl_chart.o:mathGL/mgl_chart.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c mathGL/mgl_chart.cpp $(LIBS)

xml.o:xml.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c xml.cpp $(LIBS)

tinyxml2.o:tinyxml2.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c tinyxml2.cpp $(LIBS)

random_gen.o:random_gen.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c random_gen.cpp $(LIBS)

param.o:param.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c param.cpp $(LIBS)

