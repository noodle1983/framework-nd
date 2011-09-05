PROJBASE=$(PWD)/..

INC= -I. \
     -I./include \
     -I./Connection \
     -I./Server \
     -I$(PROJBASE)/libevent/include

LIB= \
    /data/workdir/framework-nd/libevent/lib/libevent.a \

CC=g++
cc=gcc
AR=ar
ARFLAGS=-rc
CXXFLAGS=-g $(INC)
CFLAGs=-g $(INC)
LDFLAGS=-lrt



OUTDIR=.lib

#########################################
#                Target                 #
#########################################
TARGET=libNet.a
SUBDIR=Connection \
        Server 
CFILES=$(foreach DIR, $(SUBDIR), $(wildcard $(DIR)/*.c))
CPPFILES=$(foreach DIR, $(SUBDIR), $(wildcard $(DIR)/*.cpp))
TMPOBJECTS=$(patsubst %.cpp, %.o, $(CPPFILES)) \
           $(patsubst %.c, %.o, $(CFILES))
OUTSUBDIR=$(addprefix $(OUTDIR)/, $(SUBDIR))
OBJECTS=$(addprefix $(OUTDIR)/, $(TMPOBJECTS))

#########################################
#        Test    Target                 #
#########################################
TESTTARGET=test
TESTDIR=UnitTest
TESTCPPFILES=$(foreach DIR, $(TESTDIR), $(wildcard $(DIR)/*.cpp))
TMPTESTOBJECTS=$(patsubst %.cpp, %.o, $(TESTCPPFILES))
TESTOUTDIR=$(addprefix $(OUTDIR)/, $(TESTDIR))
TESTOBJECTS=$(addprefix $(OUTDIR)/, $(TMPTESTOBJECTS))

#########################################
#               Rules                   #
#########################################
$(TARGET):$(OUTDIR) $(OUTSUBDIR) $(OBJECTS)
	$(AR) $(ARFLAGS) $(OUTDIR)/$(TARGET) $(OBJECTS)

$(TESTTARGET):$(OUTDIR) $(TESTOUTDIR) $(TESTOBJECTS)
	$(CC) $(CXXFLAGS) -o $(OUTDIR)/$(TESTTARGET) $(TESTOBJECTS) $(OUTDIR)/$(TARGET) $(LIB) $(LDFLAGS)

$(OUTDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

$(TESTOUTDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

$(OUTSUBDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

$(OUTDIR)/%.o:%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OUTDIR)/%.o:%.c
	$(CC) $(CXXFLAGS) -o $@ -c $<


clean:
	@rm -rf $(OUTDIR)

