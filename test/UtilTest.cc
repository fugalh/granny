#include <gtest/gtest.h>

#include "Util.hh"

TEST(basename, relative_path) {
  EXPECT_EQ("foo", Util::basename("path/to/foo"));
}

TEST(basename, absolute_path) {
  EXPECT_EQ("foo", Util::basename("/path/to/foo"));
}

TEST(basename, no_path) {
  EXPECT_EQ("foo", Util::basename("foo"));
}

TEST(basename, suffix) {
  EXPECT_EQ("foo", Util::basename("foo.wav", true));
}

TEST(basename, path_and_suffix) {
  EXPECT_EQ("foo", Util::basename("wavs/foo.wav", true));
}

TEST(basename, del_bug) {
  EXPECT_EQ("delete", Util::basename("wavs/delete.wav", true));
}
