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

STATIC_LIB= $(PROJBASE)/.lib/libLog.a \

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
include $(PROJBASE)/build/makefile.compile.rules

