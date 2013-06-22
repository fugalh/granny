SRCS = Sndfile.cc
TEST_SRCS = test/main.cc test/Sndfile.cc

CPPFLAGS += -I/opt/local/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/opt/local/lib
LDLIBS += -lsndfile -lgtest

test: test/test
	test/test

test/test: $(SRCS) $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

.PHONY: test
