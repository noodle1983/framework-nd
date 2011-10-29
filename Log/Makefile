#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
MOD_NAME=Log
#BIN_TARGET=Log
#SHARED_TARGET=libLog.so
STATIC_TARGET=libLog.a
SUBDIR=src 

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
     -I./src \

STATIC_LIB= \

SHARED_LIB_DIR=
SHARED_LIB= -pthread

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

