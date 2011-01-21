.DELETE_ON_ERROR:

SHELL         = /bin/bash

ROOTCONFIG   := root-config
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTDICTTYPE := $(shell $(ROOTCONFIG) --dicttype)
ROOTCINT     := rootcint
ROOTLIBDIR  := $(shell $(ROOTCONFIG) --libdir)

CXX           = g++
CXXFLAGS      = -O2 -Wall -fPIC $(ROOTCFLAGS)
LD            = g++
LDFLAGS       = -O2 $(ROOTLDFLAGS)
SOFLAGS       = -shared
LIBS          = $(ROOTLIBS)

MAIN = fitMain
MAIN_SRC = fitMain.cxx
MAIN_LIBS = -L$(ROOTLIBDIR) -lCore -lCint -lTree -lHist -lRooFit
MAIN_INCLUDES = -Iincludes/


.PHONY: all lib clean 

all: $(MAIN)
$(MAIN): $(MAIN_SRC)
	        $(CXX) $(CXXFLAGS) $(LDFLAGS) $(MAIN_INCLUDES) $(MAIN_SRC) $(MAIN_LIBS) -o $@
		        @echo "$@ done"

clean:
	        @rm -f $(MAIN) 


-include Makefile_me


