#########################################
#                top dir                #
#########################################
PROJBASE=$(PWD)/..

#########################################
#                target                 #
#########################################
TARGET=
SUBDIR=

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. -IAutoMsg -IManMsg \
		 -I$(PROJBASE)/Log/include

STATIC_LIB=$(PROJBASE)/Log/.lib/libLog.a 
SHARED_LIB_DIR=
SHARED_LIB=

include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

