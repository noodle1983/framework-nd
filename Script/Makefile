#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
SUBDIR=Lua \
	   LuaObject

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
	 -I$(PROJBASE)/Log/include \
	 -I$(PROJBASE)/DesignPattern/include

STATIC_LIB= $(PROJBASE)/.lib/libLog.a \

SHARED_LIB_DIR=
SHARED_LIB= -lcrypt -lrt -pthread -ldl

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
include $(PROJBASE)/build/makefile.staticlib.lua.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

