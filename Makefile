#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)

#########################################
#                target                 #
#########################################
SUBDIR= Log \
		Config \
		Processor \
		Net \
		Message \
		Fsm \
		Script \

RELEASE_DIR = $(PROJBASE)/release
#########################################
#                rules                  #
#########################################
include build/makefile.compiler

all: $(SUBDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make -C $$DIR || exit -1; \
	done                  

test: $(SUBDIR) $(OUTDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make test -C $$DIR || exit -1; \
	done                  

clean: $(SUBDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make clean -C $$DIR || exit -1;  \
	done                  

release: all
	@rm -rf $(RELEASE_DIR)
	@mkdir $(RELEASE_DIR) \
		$(RELEASE_DIR)/include \
		$(RELEASE_DIR)/lib 
	@for DIR in $(SUBDIR);  \
	do                     \
		cd $(RELEASE_DIR)/include && ln -s ../../$$DIR/include/* . && cd $(PROJBASE) ; \
		cd $(RELEASE_DIR)/lib && ln -s ../../$$DIR/.lib/*.a . && cd $(PROJBASE); \
	done                  
	@cd $(RELEASE_DIR)/lib && \
	$(AR) $(ARFLAGS) libBase.a `ls *.a` && \
	cd $(PROJBASE)

.PHONY:all test clean release
