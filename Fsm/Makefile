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
INC_DIR= -I. -Isrc \
		-I../Net/include \
		-I../Log/include \
		-I../Processor/include

STATIC_LIB= \
    $(PROJBASE)/Processor/.lib/libProcessor.a \
	$(PROJBASE)/Log/.lib/libLog.a \
	$(PROJBASE)/Net/.lib/libNet.a \

SHARED_LIB_DIR=
SHARED_LIB=-pthread -lrt

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
include $(PROJBASE)/build/makefile.staticlib.libevent.linux
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.staticlib.rules

