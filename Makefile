SHELL=/bin/bash

# compilers
CXX = g++
LD = g++
OPTIM = -O3
CXXFLAGS = -std=c++17 -s -Wall
DEPFLAGS = -MT $@ -MMD -MP -MF
TEST_LDFLAGS = -lgtest -lgtest_main -lpthread
GCOVFLAGS = -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline

MAIN_TARGET = simplebf
LIB_TARGET = libsimplebf.so.0.0.1
TEST_TARGET = test_simplebf
GCOV_TARGET = gcov
LCOV_TARGET = lcov

# sources
SRCDIR = src
OBJDIR = build
DEPDIR = $(OBJDIR)
SRCS = $(wildcard $(SRCDIR)/*.cc)
OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.cc=.o))

# main
MAINDIR = main
MAIN_SRCS = $(MAINDIR)/main.cc
MAIN_OBJS = $(subst $(MAINDIR)/,$(OBJDIR)/,$(MAIN_SRCS:.cc=.o))
DEPS = $(OBJS:.o=.d) $(MAIN_OBJS:.o=.d)

# lib
LIB_OBJDIR = build/lib
LIB_DEPDIR = $(LIB_OBJDIR)
LIB_OBJS = $(subst $(SRCDIR)/,$(LIB_OBJDIR)/,$(SRCS:.cc=.o))
LIB_DEPS = $(LIB_OBJS:.o=.d)

# test
TEST_SRCDIR = test
TEST_OBJDIR = build/test
TEST_DEPDIR = $(TEST_OBJDIR)
TEST_SRCS = $(wildcard $(TEST_SRCDIR)/*.cc)
TEST_TARGET_OBJS = $(subst $(SRCDIR)/,$(TEST_OBJDIR)/,$(SRCS:.cc=.o))
TEST_OBJS = $(subst $(TEST_SRCDIR)/,$(TEST_OBJDIR)/,$(TEST_SRCS:.cc=.o))
TEST_DEPS = $(TEST_TARGET_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

# gcov
GCOV_OBJDIR = build/gcov
GCOV_DEPDIR = $(GCOV_OBJDIR)
GCOV_OBJS = $(subst $(SRCDIR)/,$(GCOV_OBJDIR)/,$(SRCS:.cc=.o))
GCOV_TESTOBJS = $(subst $(TEST_SRCDIR)/,$(GCOV_OBJDIR)/,$(TEST_SRCS:.cc=.o))
GCOV_DEPS = $(GCOV_OBJS:.o=.d) $(GCOV_TESTOBJS:.o=.d)

# includes
INCLUDES = -I./include
TEST_INCLUDES = -I/usr/local/include

# lcov
LCOVDIR = lcov
COVERAGE = coverage.info

# doxygen
DOXYGEN = doxygen
DOCDIR = doxygen
INDEXPATH = $(DOXYGEN)/html/index.html

.PHONY: all build install uninstall clean test gcov lcov docs

build: $(MAIN_TARGET)

lib: $(LIB_TARGET)

install:
	@install -d /usr/local/include/simplebf > /dev/null 2>&1
	@install include/simplebf/* /usr/local/include/simplebf/
	@install $(LIB_OBJDIR)/$(LIB_TARGET) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LIB_TARGET) /usr/local/lib/libsimplebf.so

uninstall:
	@rm -rf /usr/local/include/simplebf
	@rm -f /usr/local/lib/$(LIB_TARGET)
	@rm -f /usr/local/lib/libsimplebf.so

test: $(TEST_TARGET)

all: build lib lcov docs

$(MAIN_TARGET): $(MAIN_OBJS) $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS) $(TEST_TARGET_OBJS)
	$(LD) -o $@ $^ $(TEST_LDFLAGS) 

$(GCOV_TARGET): $(GCOV_TESTOBJS) $(GCOV_OBJS)
	$(LD) $(GCOVFLAGS) -o $(TEST_TARGET) $^ $(TEST_LDFLAGS) 
	./$(TEST_TARGET)

$(LIB_TARGET): $(LIB_OBJS)
	$(LD) -shared -o $(LIB_OBJDIR)/$@ $^ $(LDFLAGS)

$(MAIN_TARGET): DEPFLAGS += $(DEPDIR)/$*.d
$(TEST_TARGET): DEPFLAGS += $(TEST_DEPDIR)/$*.d
$(LIB_TARGET): DEPFLAGS += $(LIB_DEPDIR)/$*.d
$(GCOV_TARGET): DEPFLAGS += $(GCOV_DEPDIR)/$*.d

CXXFLAGS += $(DEPFLAGS)

$(GCOV_TARGET): CXXFLAGS += $(GCOVFLAGS)

$(TEST_TARGET): INCLUDES += $(TEST_INCLUDES)
$(GCOV_TARGET): INCLUDES += $(TEST_INCLUDES)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.cc $(OBJDIR)/%.d
	@mkdir -p $(dir $(OBJS))
	$(CXX) $(CXXFLAGS) $(OPTIM) $(INCLUDES) -c $< -o $@

$(MAIN_OBJS): $(OBJDIR)/%.o: $(MAINDIR)/%.cc $(OBJDIR)/%.d
	@mkdir -p $(dir $(OBJS))
	$(CXX) $(CXXFLAGS) $(OPTIM) $(INCLUDES) -c $< -o $@

$(DEPS):

$(LIB_OBJS): $(LIB_OBJDIR)/%.o: $(SRCDIR)/%.cc $(LIB_OBJDIR)/%.d
	@mkdir -p $(dir $(LIB_OBJS))
	$(CXX) $(CXXFLAGS) -fPIC $(OPTIM) $(INCLUDES) -c $< -o $@

$(LIB_DEPS):

$(TEST_OBJS): $(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%.cc $(TEST_OBJDIR)/%.d
	@mkdir -p $(dir $(TEST_TARGET_OBJS))
	$(CXX) $(CXXFLAGS) $(OPTIM) $(INCLUDES) -c $< -o $@

$(TEST_TARGET_OBJS): $(TEST_OBJDIR)/%.o: $(SRCDIR)/%.cc $(TEST_OBJDIR)/%.d
	@mkdir -p $(dir $(TEST_TARGET_OBJS))
	$(CXX) $(CXXFLAGS) $(OPTIM) $(INCLUDES) -c $< -o $@

$(TEST_DEPS):

$(GCOV_TESTOBJS): $(GCOV_OBJDIR)/%.o: $(TEST_SRCDIR)/%.cc $(GCOV_OBJDIR)/%.d
	@mkdir -p $(dir $(GCOV_OBJS))
	$(CXX) $(CXXFLAGS) $(OPTIM) $(INCLUDES) -c $< -o $@

$(GCOV_OBJS): $(GCOV_OBJDIR)/%.o: $(SRCDIR)/%.cc $(GCOV_OBJDIR)/%.d
	@mkdir -p $(dir $(GCOV_OBJS))
	$(CXX) $(CXXFLAGS) $(OPTIM) $(INCLUDES) -c $< -o $@

$(GCOV_DEPS):

$(LCOV_TARGET): $(GCOV_TARGET)
	lcov --capture --directory . --output-file $(COVERAGE)
	lcov --remove $(COVERAGE) **include/c++/** --output-file $(COVERAGE)
	lcov --remove $(COVERAGE) **bits** --output-file $(COVERAGE)
	lcov --remove $(COVERAGE) **gtest*.h --output-file $(COVERAGE)
	lcov --remove $(COVERAGE) **gtest*.cc --output-file $(COVERAGE)
	genhtml $(COVERAGE) --output-directory $(LCOVDIR)
	@rm -f *.gcno
	@rm -f *.gcda
	@rm -f $(COVERAGE)
	@echo -n -e ""
	@echo $(LCOVDIR)/index.html

docs:
	$(DOXYGEN)
	@echo $(INDEXPATH)

clean:
	@rm -f $(MAIN_TARGET)
	@rm -f $(TEST_TARGET)
	@rm -rf $(OBJDIR)
	@rm -rf $(LCOVDIR)
	@rm -rf $(DOCDIR)

-include $(DEPS) $(TEST_DEPS) $(GCOV_DEPS)
