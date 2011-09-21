PROJBASE=$(PWD)/..


#########################################
#                Target                 #
#########################################
TARGET=libNet.a
SUBDIR=Connection \
        Server  \
        Reactor \
        Protocol \
        Buffer

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. \
     -I./include \
     -I$(PROJBASE)/Processor/include \
     -I$(PROJBASE)/Log/include \

STATIC_LIB= \
    $(PROJBASE)/Processor/.lib/libProcessor.a \
    $(PROJBASE)/Log/.lib/libLog.a \

SHARED_LIB_DIR=
SHARED_LIB=-lrt

include $(PROJBASE)/build/makefile.staticlib.boost.linux
include $(PROJBASE)/build/makefile.staticlib.libevent.linux
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

