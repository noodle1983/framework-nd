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
INC_DIR= -I. -I./include
STATIC_LIB= 
SHARED_LIB_DIR=
SHARED_LIB=-ldl

include $(PROJBASE)/build/makefile.staticlib.configAndLog.linux
include $(PROJBASE)/build/makefile.staticlib.mysql.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

