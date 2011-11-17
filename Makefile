#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)

#########################################
#                target                 #
#########################################
TARGET=libBase
SUBDIR= Log \
		Config \
		Processor \
		Net \
		Message \
		Fsm \
		Script \
		Data  \
		DesignPattern \

RELEASE_DIR = $(PROJBASE)/release
#########################################
#                rules                  #
#########################################
include build/makefile.compiler

#########################################
#      header and lib dependancy        #
#########################################
SHARED_LIB_DIR=
SHARED_LIB=-lrt

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
include $(PROJBASE)/build/makefile.staticlib.libevent.linux
include $(PROJBASE)/build/makefile.staticlib.log4cplus.linux
include $(PROJBASE)/build/makefile.staticlib.lua.linux

#########################################
#            rules                      #
#########################################
all: $(SUBDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make -C $$DIR || exit -1; \
	done                  

test: all $(SUBDIR) $(OUTDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make test -C $$DIR || exit -1; \
	done                  

clean: $(SUBDIR)
	@rm -rf $(RELEASE_DIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make clean -C $$DIR || exit -1;  \
	done                  

OBJECT_LIST_FILES = $(addprefix $(PROJBASE)/.lib/, $(addsuffix .objectlist, $(SUBDIR)))
ALL_OBJECT_LIST = `cat $(OBJECT_LIST_FILES)`
release: all
	@rm -rf $(RELEASE_DIR)
	@mkdir $(RELEASE_DIR) \
		$(RELEASE_DIR)/include \
		$(RELEASE_DIR)/lib 
	@for DIR in $(SUBDIR);  \
	do                     \
		cd $(RELEASE_DIR)/include/ && \
		cp ../../$$DIR/include/* . && \
		cd $(PROJBASE) ; \
	done                  
	cd $(RELEASE_DIR)/lib && \
	cp ../../.lib/*.a . && \
	$(AR) $(ARFLAGS) $(TARGET).a $(ALL_OBJECT_LIST) && \
	$(RANLIB) $(TARGET).a  \
	#$(CC) $(SHAREDFLAG)  -o $(TARGET).so $(ALL_OBJECT_LIST) $(STATIC_LIB) $(SHSRED_LIB)


	cd $(PROJBASE)

.PHONY:all test clean release
