#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
TARGET=libScript.a
SUBDIR=ruby \
	   swig

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
     -I./ruby \
     -I./swig \

STATIC_LIB= \

SHARED_LIB_DIR=
SHARED_LIB= -lcrypt -lrt -pthread -ldl

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
include $(PROJBASE)/build/makefile.staticlib.ruby.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

