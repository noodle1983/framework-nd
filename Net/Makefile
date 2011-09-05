PROJBASE=/data/workdir/framework-nd/

INC= -I. \
	 -I./include \
	 -I./Connection \
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



TARGET=libNet.a
OUTDIR=.lib
SUBDIR=Connection \
		Server 


CFILES=$(foreach DIR, $(SUBDIR), $(wildcard $(DIR)/*.c))
CPPFILES=$(foreach DIR, $(SUBDIR), $(wildcard $(DIR)/*.cpp))
TMPOBJECTS=$(patsubst %.cpp, %.o, $(CPPFILES)) \
           $(patsubst %.c, %.o, $(CFILES))
OUTSUBDIR=$(addprefix $(OUTDIR)/, $(SUBDIR))
OBJECTS=$(addprefix $(OUTDIR)/, $(TMPOBJECTS))

$(TARGET):$(OUTDIR) $(OUTSUBDIR) $(OBJECTS)
	$(AR) $(ARFLAGS) $(OUTDIR)/$(TARGET) $(OBJECTS) $(LIB)

$(OUTDIR):
	@if [ ! -d $(OUTDIR) ]; then mkdir $(OUTDIR); fi;

$(OUTSUBDIR):
	@if [ ! -d $@ ]; then mkdir $@; fi;

$(OUTDIR)/%.o:%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OUTDIR)/%.o:%.c
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf $(OUTDIR)

