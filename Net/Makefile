PROJBASE=$(PWD)/..

INC_DIR= -I. \
     -I./include \
     -I$(PROJBASE)/boost/include \
     -I$(PROJBASE)/Processor/include \
     -I$(PROJBASE)/libevent/include

STATIC_LIB= \
    $(PROJBASE)/libevent/lib/libevent.a \
    $(PROJBASE)/libevent/lib/libevent_pthreads.a \
    $(PROJBASE)/Processor/.lib/libProcessor.a \
    $(PROJBASE)/boost/lib/libboost_thread.a \

SHARED_LIB_DIR=
SHARED_LIB=-lrt


#########################################
#                Target                 #
#########################################
TARGET=libNet.a
SUBDIR=Connection \
        Server  \
        Reactor \
        Protocol \
        Buffer

include $(PROJBASE)/build/makefile.compiler
include $(PROJBASE)/build/makefile.staticlib.rules

