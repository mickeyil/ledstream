
CC := g++
CFLAGS := -g -Wall -Wno-sign-compare -Wno-deprecated-declarations -Wno-unused-function -std=c++0x -I src -I .
LIBS := -lm  #-lyaml-cpp -lboost_program_options -lzmq 

ifdef DEBUG
  CFLAGS += -O0
else
  CFLAGS += -O3
endif

GIT_VERSION := $(shell git describe --abbrev=7 --dirty --always --tags)

CFLAGS += -DVERSION=\"$(GIT_VERSION)\"

# filter source files containing main() function
MAIN_SRCS = src/rgb2hsv.cc

OBJECTS_SRC = $(wildcard src/*.cc)
OBJECTS_SRC_FILTERED = $(filter-out $(MAIN_SRCS), $(OBJECTS_SRC))

OBJECTS = $(OBJECTS_SRC_FILTERED:.cc=.o)

HEADERS  = $(wildcard src/*.h)

OBJDIR    := obj
OBJECTS   := $(addprefix $(OBJDIR)/,$(OBJECTS))
TARGETOBJ := $(OBJDIR)/src/

TARGETDIR := bin
TRG_rgb2hsv = $(TARGETDIR)/rgb2hsv


.PHONY: default all clean
.PRECIOUS: $(OBJECTS)

default: all

.PHONY: rgb2hsv
rgb2hsv: $(TRG_rgb2hsv)

all: rgb2hsv


%.o: %.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


$(TRG_rgb2hsv): $(OBJECTS) $(TARGETOBJ)/rgb2hsv.o
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(TARGETOBJ)/rgb2hsv.o $(LIBS) -o $@

clean:
	rm -rf $(OBJDIR)
	rm -f $(TRG_rgb2hsv)
