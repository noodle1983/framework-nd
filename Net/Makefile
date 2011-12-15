PROJBASE=$(CURDIR)/..


#########################################
#                Target                 #
#########################################
SUBDIR=Connection \
        Server  \
		Client \
        Reactor \
        Protocol \
        TelnetProtocol \
        Buffer \
        Pcap \

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. \
     -I./include \
     -I$(PROJBASE)/Processor/include \
     -I$(PROJBASE)/Log/include \
     -I$(PROJBASE)/Data/include \
     -I$(PROJBASE)/Utility/include \

STATIC_LIB= \
    $(PROJBASE)/.lib/libProcessor.a \
    $(PROJBASE)/.lib/libLog.a \
    $(PROJBASE)/.lib/libConfig.a \
    $(PROJBASE)/.lib/libUtility.a \

SHARED_LIB_DIR=
SHARED_LIB=-lrt -lpcap

include $(PROJBASE)/build/makefile.staticlib.configAndLog.linux
include $(PROJBASE)/build/makefile.staticlib.libevent.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

