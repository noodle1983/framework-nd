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
INC_DIR= -I. -Iinclude
STATIC_LIB= 
SHARED_LIB_DIR=
SHARED_LIB=

include $(PROJBASE)/build/makefile.staticlib.glib.linux
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

