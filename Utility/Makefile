#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
SUBDIR=src 

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
     -I./src \
     -I$(PROJBASE)/Log/include \
     -I$(PROJBASE)/Net/include \
     -I$(PROJBASE)/Data/include \
     -I$(PROJBASE)/Config/include \
     -I$(PROJBASE)/Processor/include \

STATIC_LIB= $(PROJBASE)/.lib/libLog.a \
	$(PROJBASE)/.lib/libConfig.a \

SHARED_LIB_DIR=
SHARED_LIB= -pthread -lrt

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.rapidxml.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
include $(PROJBASE)/build/makefile.staticlib.libevent.linux
include $(PROJBASE)/build/makefile.staticlib.google_breakpad.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

