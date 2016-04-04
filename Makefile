SRCS := $(wildcard *.cc) \
	$(wildcard **/*.cc)
OBJS := $(SRCS:.cc=.o)
DEPS := $(OBJS:.o=.d)
EXEC ?= main

CXXFLAGS ?= -std=c++14 -Wall --pedantic -I.
LDFLAGS ?= -lglut -lGL -lGLU
CXX ?= g++
RM ?= rm -rf

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.d: %.cc
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(EXEC)

-include $(DEPS)
