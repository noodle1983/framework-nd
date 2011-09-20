#########################################
#                top dir                #
#########################################
PROJBASE=$(PWD)/..

#########################################
#                target                 #
#########################################
TARGET=libLog.a
SUBDIR=src 

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
     -I./src \
     -I$(PROJBASE)/boost/include \
     -I$(PROJBASE)/log4cplus/include

STATIC_LIB= \
		$(PROJBASE)/boost/lib/libboost_thread.a \

SHARED_LIB_DIR=
SHARED_LIB= -pthread

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

