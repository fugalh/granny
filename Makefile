SRCS = src/Sndfile.cc
TEST_SRCS = src/test/main.cc src/test/SndfileTest.cc

#CXX = clang++
CPPFLAGS += -I/opt/local/include -g -O0
CXXFLAGS += -std=c++11
LDFLAGS += -L/opt/local/lib
LDLIBS += -lsndfile -lgtest

test: tests
	./tests

tests: $(SRCS) $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

clean:
	rm -f tests granny

.PHONY: all test clean
