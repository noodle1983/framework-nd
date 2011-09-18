PROJBASE=$(PWD)/..

INC= -I. \
     -I./include \
     -I$(PROJBASE)/boost/include \
     -I$(PROJBASE)/Processor/include \
     -I$(PROJBASE)/libevent/include

LIB= \
    $(PROJBASE)/libevent/lib/libevent.a \
    $(PROJBASE)/libevent/lib/libevent_pthreads.a \
    $(PROJBASE)/Processor/.lib/libProcessor.a \
    $(PROJBASE)/boost/lib/libboost_thread.a \


CC=g++
cc=gcc
AR=ar
ARFLAGS=-rc
CXXFLAGS=-Wall -g $(INC) 
CFLAGs=-Wall -g $(INC)
LDFLAGS=-lrt


OUTDIR=.lib

#########################################
#                Target                 #
#########################################
TARGET=libNet.a
SUBDIR=Connection \
        Server  \
        Reactor \
        Protocol \
        Buffer

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
#               Depend                  #
#########################################
DEPENDFILES=$(patsubst %.o, %.d, $(OBJECTS))
DEPENDTARGET=$(patsubst %.d, %.d.tmp, $(DEPENDFILES))

#########################################
#               Rules                   #
#########################################
all:$(TARGET)

$(TARGET):depend $(OUTDIR) $(OUTSUBDIR) $(OBJECTS)
	$(AR) $(ARFLAGS) $(OUTDIR)/$(TARGET) $(OBJECTS)

$(TESTTARGET):$(TARGET) $(TESTOUTDIR) $(TESTOBJECTS)
	$(CC) $(CXXFLAGS) -o $(OUTDIR)/$(TESTTARGET) $(TESTOBJECTS) $(OUTDIR)/$(TARGET) $(LIB) $(LDFLAGS)

$(OUTDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

$(TESTOUTDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

$(OUTSUBDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

-include $(DEPENDFILES)
$(OUTDIR)/%.d.tmp:%.cpp
	@echo "building dependancy for $<..."
	@set -e; rm -f $@ $(OUTDIR)/$*.d; \
	$(CC) $(CXXFLAGS) -MM -c $< > $@; \
	sed 's,\(.*\)\.o[ :]*,$(OUTDIR)/$*.o $@: ,g' <$@ >$(OUTDIR)/$*.d; 

$(OUTDIR)/%.o:%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OUTDIR)/%.o:%.c
	$(CC) $(CXXFLAGS) -o $@ -c $<

depend: $(OUTDIR) $(OUTSUBDIR) $(DEPENDTARGET)

.PHONY:clean depend
clean:
	@rm -rf $(OUTDIR)

