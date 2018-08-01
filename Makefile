
CC := g++
CFLAGS := -g -Wall -Wno-sign-compare -Wno-deprecated-declarations -Wno-unused-function -Wno-int-in-bool-context -std=c++0x -I src -I ../Eigen -I ../LedBurnBBB
LIBS := -lm  #-lyaml-cpp -lboost_program_options -lzmq 

ifdef DEBUG
  CFLAGS += -O0
else
  CFLAGS += -O3 -DNDEBUG
endif

GIT_VERSION := $(shell git describe --abbrev=7 --dirty --always --tags)

CFLAGS += -DVERSION=\"$(GIT_VERSION)\"

# filter source files containing main() function
MAIN_SRCS = src/test_eigen.cc src/test_ledscape.cc src/test_linpix.cc src/test_animation.cc

OBJECTS_SRC = $(wildcard src/*.cc)
OBJECTS_SRC_FILTERED = $(filter-out $(MAIN_SRCS), $(OBJECTS_SRC))

OBJECTS = $(OBJECTS_SRC_FILTERED:.cc=.o)

HEADERS  = $(wildcard src/*.h)

OBJDIR    := obj
OBJECTS   := $(addprefix $(OBJDIR)/,$(OBJECTS))
TARGETOBJ := $(OBJDIR)/src/

TARGETDIR := bin
#TRG_rgb2hsv = $(TARGETDIR)/rgb2hsv
TRG_test_animation = $(TARGETDIR)/test_animation
TRG_test_linpix = $(TARGETDIR)/test_linpix
TRG_test_eigen = $(TARGETDIR)/test_eigen
TRG_test_ledscape = $(TARGETDIR)/test_ledscape


.PHONY: default all clean
.PRECIOUS: $(OBJECTS)

default: all

.PHONY: test_eigen test_ledscape test_linpix test_animation
#rgb2hsv: $(TRG_rgb2hsv)
test_animation: $(TRG_test_animation)
test_linpix: $(TRG_test_linpix)
test_eigen: $(TRG_test_eigen)
test_ledscape: $(TRG_test_ledscape)

all: test_eigen test_linpix test_animation


%.o: %.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


#$(TRG_rgb2hsv): $(OBJECTS) $(TARGETOBJ)/rgb2hsv.o
#	@mkdir -p $(@D)
#	$(CC) $(OBJECTS) $(TARGETOBJ)/rgb2hsv.o $(LIBS) -o $@

$(TRG_test_animation): $(OBJECTS) $(TARGETOBJ)/test_animation.o
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(TARGETOBJ)/test_animation.o $(LIBS) -o $@

$(TRG_test_linpix): $(OBJECTS) $(TARGETOBJ)/test_linpix.o
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(TARGETOBJ)/test_linpix.o $(LIBS) -o $@


$(TRG_test_eigen): $(OBJECTS) $(TARGETOBJ)/test_eigen.o
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(TARGETOBJ)/test_eigen.o $(LIBS) -o $@

$(TRG_test_ledscape): $(OBJECTS) $(TARGETOBJ)/test_ledscape.o
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(TARGETOBJ)/test_ledscape.o $(LIBS) -o $@

clean:
	rm -rf $(OBJDIR)
#	rm -f $(TRG_rgb2hsv)
	rm -f $(TRG_test_eigen)
	rm -f $(TRG_test_animation)
	rm -f $(TRG_test_linpix)
	rm -f $(TRG_test_ledscape)
