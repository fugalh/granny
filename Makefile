SRCS = Sndfile.cc
TEST_SRCS = test/main.cc test/SndfileTest.cc

#CXX = clang++
CPPFLAGS += -I/opt/local/include -g -O0
CXXFLAGS += -std=c++11
LDFLAGS += -L/opt/local/lib
LDLIBS += -lsndfile -lgtest

test: test/test
	test/test

test/test: $(SRCS) $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

clean:
	rm -f test/test granny

.PHONY: test clean
