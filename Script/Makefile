#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
TARGET=libScript.a
SUBDIR=Lua 

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
	 -I$(PROJBASE)/Log/include

STATIC_LIB= $(PROJBASE)/Log/.lib/libLog.a \

SHARED_LIB_DIR=
SHARED_LIB= -lcrypt -lrt -pthread -ldl

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
include $(PROJBASE)/build/makefile.staticlib.lua.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

