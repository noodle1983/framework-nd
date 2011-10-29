PROJBASE=$(CURDIR)/..


#########################################
#                Target                 #
#########################################
MOD_NAME=Net
#BIN_TARGET=Net
#SHARED_TARGET=libNet.so
STATIC_TARGET=libNet.a
SUBDIR=Connection \
        Server  \
		Client \
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
    $(PROJBASE)/.lib/libProcessor.a \
    $(PROJBASE)/.lib/libLog.a \

SHARED_LIB_DIR=
SHARED_LIB=-lrt

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
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

