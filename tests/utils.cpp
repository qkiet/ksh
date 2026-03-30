#include <gtest/gtest.h>
#include "utils.h"

// Test the strip function
TEST(UtilTest, Strip) {
  std::string str = "hello  world";
  std::string dst = strip(str, ' ');
  EXPECT_EQ(dst, "hello world");
  std::string str2 = " hello  world  ";
  std::string dst2 = strip(str2, ' ');
  EXPECT_EQ(dst2, "hello world");
}

// Test the split_string function
TEST(UtilTest, SplitString) {
  std::string str = "hello world";
  std::vector<std::string> vec;
  vec = split_string(str, ' ');
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0], "hello");
  EXPECT_EQ(vec[1], "world");
  std::string str2 = "";
  std::vector<std::string> vec2;
  vec2 = split_string(str2, ' ');
  EXPECT_EQ(vec2.size(), 0);
}

TEST(UtilTest, FreeCharPointerList) {
    char *ptr_list[3];
    std::cout << "Freeing list with pointer " << &ptr_list << " with length 3" << std::endl;
    ptr_list[0] = (char *) malloc(5);
    printf("ptr_list[0] = %p\n", ptr_list[0]);
    ptr_list[1] = (char *) malloc(5);
    printf("ptr_list[1] = %p\n", ptr_list[1]);
    ptr_list[2] = (char *) malloc(5);
    printf("ptr_list[2] = %p\n", ptr_list[2]);
    free_char_pointer_list((char ***)&ptr_list, 3);
    EXPECT_EQ(ptr_list[0], nullptr);
    EXPECT_EQ(ptr_list[1], nullptr);
    EXPECT_EQ(ptr_list[2], nullptr);
  }


TEST(UtilTest, VectorToNullTermCharPointerList) {
  std::vector<std::string> vec = {"hello", "world", "test"};
  size_t len;
  char **ptr_list = vector_to_null_term_char_pointer_list(vec, &len);
  EXPECT_EQ(len, 4);
  EXPECT_EQ(std::string(ptr_list[0]), std::string("hello"));
  EXPECT_EQ(std::string(ptr_list[1]), std::string("world"));
  EXPECT_EQ(std::string(ptr_list[2]), std::string("test"));
  EXPECT_EQ(ptr_list[3], nullptr);
  free_char_pointer_list((char ***)&ptr_list, len);
  free(ptr_list);
}