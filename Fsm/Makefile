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
INC_DIR= -I. -Iinclude \
		-I../Net/include \
		-I../Log/include \
		-I../Processor/include

STATIC_LIB= \
    $(PROJBASE)/.lib/libProcessor.a \
	$(PROJBASE)/.lib/libLog.a \
	$(PROJBASE)/.lib/libNet.a \

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
include $(PROJBASE)/build/makefile.compile.rules

