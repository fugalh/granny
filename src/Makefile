SRCS = Sndfile.cc
TEST_SRCS = $(SRCS) test/main.cc test/SndfileTest.cc test/SndbufTest.cc test/OSCTest.cc
HEADERS = Sndfile.hh Sndbuf.hh OSC.hh

CPPFLAGS += -Wall -Werror -I/opt/local/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/opt/local/lib
LDLIBS += -lsndfile -lgtest -llo

test: tests
	./tests

tests: $(TEST_SRCS) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(TEST_SRCS) -g -o $@

clean:
	rm -f tests granny

.PHONY: all test clean
