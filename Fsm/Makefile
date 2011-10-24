#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
TARGET=libFsm.a
SUBDIR=src

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. -Isrc 
STATIC_LIB= 
SHARED_LIB_DIR=
SHARED_LIB=

include $(PROJBASE)/build/makefile.boost.header
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

