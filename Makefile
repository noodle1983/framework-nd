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


#########################################
#                rules                  #
#########################################

all: $(SUBDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make -C $$DIR;     \
	done                  

test: $(SUBDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make test -C $$DIR;     \
	done                  

clean: $(SUBDIR)
	@for DIR in $(SUBDIR);  \
	do                     \
		make clean -C $$DIR;     \
	done                  

.PHONY:all test clean
