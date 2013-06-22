SRCS = Sndfile.cc
TEST_SRCS = $(SRCS) test/main.cc test/SndfileTest.cc test/SndbufTest.cc
HEADERS = Sndfile.hh Sndbuf.hh

CXX ?= clang++
CPPFLAGS += -Wall -Werror -I/opt/local/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/opt/local/lib
LDLIBS += -lsndfile -lgtest

test: tests
	./tests

tests: $(TEST_SRCS) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(TEST_SRCS) -g -o $@

clean:
	rm -f tests granny

.PHONY: all test clean
