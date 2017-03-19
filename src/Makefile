
CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_PRINTSUPPORT_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -g -D_REENTRANT -fPIE $(DEFINES) -std=c++11 -std=gnu++11 -fext-numeric-literals -fpermissive
INCPATH       = -I. -I../include -I../include/QCustomPlot -I../include/mathGL
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = -lgmp -lgmpxx -lmgl -lpthread -lglpk
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f
MKDIR         = mkdir -p
TARGET	      = ChartMaker

ChartMaker:ChartMaker.o sched_result.o data_extractor.o mgl_chart.o  
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $(TARGET) ChartMaker.o sched_result.o data_extractor.o mgl_chart.o  $(LFLAGS) $(LIBS)

ChartMaker.o:ChartMaker.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c ChartMaker.cpp $(LIBS)

sched_result.o:sched_result.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c sched_result.cpp $(LIBS)

data_extractor.o:data_extractor.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c data_extractor.cpp $(LIBS)

mgl_chart.o:mathGL/mgl_chart.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c mathGL/mgl_chart.cpp $(LIBS)
