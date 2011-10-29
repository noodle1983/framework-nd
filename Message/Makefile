#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
MOD_NAME=Message
#BIN_TARGET=Gb
#SHARED_TARGET=libGb.so
STATIC_TARGET=libGb.a
SUBDIR=

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. -IAutoMsg -IManMsg \
		 -I$(PROJBASE)/Log/include

STATIC_LIB=$(PROJBASE)/.lib/libLog.a 
SHARED_LIB_DIR=
SHARED_LIB= -pthread

include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
include $(PROJBASE)/build/makefile.staticlib.glib.linux
include $(PROJBASE)/build/makefile.boost.header
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules
