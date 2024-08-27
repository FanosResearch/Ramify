# RAMBench currently supports g++ 5.1+ or clang++ 3.4+.  It will NOT work with
# g++ 4.x due to an internal compiler error when processing lambda functions.
CXX := clang++
#CXX := g++

SRCDIR := src
OBJDIR := obj
MAIN := $(SRCDIR)/Main.cpp
SRCS := $(filter-out $(MAIN), $(wildcard $(SRCDIR)/*.cpp))
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

#FMT_LIB_DIR=ext/fmt/include/
INI_LIB_DIR=ext/headers/
#JSON_LIB_DIR=ext/headers/
#ARGS_LIB_DIR=ext/headers/

#NC=-Isrc/ -I$(FMT_LIB_DIR) -I$(INI_LIB_DIR) -I$(ARGS_LIB_DIR) -I$(JSON_LIB_DIR)
INC=-Isrc/ -I$(INI_LIB_DIR)

CXXFLAGS := -Wall -O3 -fPIC -std=c++11 $(INC) 

.PHONY: all clean depend

all: depend RAMBench

clean:
	rm -f RAMBench
	rm -rf $(OBJDIR)

depend: $(OBJDIR)/.depend

$(OBJDIR)/.depend: $(SRCS)
	@mkdir -p $(OBJDIR)
	@rm -f $(OBJDIR)/.depend
	@$(foreach SRC, $(SRCS), $(CXX) $(CXXFLAGS) -DRAMBench -MM -MT $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC)) $(SRC) >> $(OBJDIR)/.depend ;)

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJDIR)/.depend
endif


RAMBench: $(MAIN) $(OBJS) $(SRCDIR)/*.h | depend
	$(CXX) $(CXXFLAGS) -DRAMBench -o $@ $(MAIN) $(OBJS)

libRAMBench.a: $(OBJS)
	libtool -static -o $@ $(OBJS)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -DRAMBench -c -o $@ $<
