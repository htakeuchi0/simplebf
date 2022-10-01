/**
 * @file gtest_util.cc
 * @brief ユーティリティ関数に対するテスト．
 */

#include <gtest/gtest.h>
#include "simplebf/util.h"

namespace {

/**
 * ユーティリティ関数のテストケース．
 */
class UtilTest : public ::testing::Test {
};

/**
 * 異なる値が異なるハッシュ値になることを確認する．
 */
TEST_F(UtilTest, Normal) {
  auto hash0 = sbf::hash::Djb2("a");
  auto hash1 = sbf::hash::Djb2("aa");
  auto hash2 = sbf::hash::Djb2("aaa");
  EXPECT_NE(hash0, hash1);
  EXPECT_NE(hash1, hash2);
  EXPECT_NE(hash2, hash0);
}

/**
 * 空文字列で初期値となることを確認する．
 */
TEST_F(UtilTest, Empty) {
  auto actual = sbf::hash::Djb2("");
  auto expect = 5381;
  EXPECT_EQ(expect, actual);
}

} // namespace


