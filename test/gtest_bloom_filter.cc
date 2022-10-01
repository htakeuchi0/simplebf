/**
 * @file gtest_bloom_filter.cc
 * @brief Bloom filter に対するテスト．
 */

#include <gtest/gtest.h>
#include "simplebf/bloom_filter.h"

namespace {

/**
 * Bloom filter のテストケース．
 */
class BloomFilterTest : public ::testing::Test {
};

/**
 * デフォルトコンストラクタで要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, NormalDefaultConstructor) {
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf;
  bf.Insert(std::string("a"));
  bf.Insert(std::string("b"));
  bf.Insert(std::string("c"));
  EXPECT_TRUE(bf.Contains(std::string("a")));
  EXPECT_TRUE(bf.Contains(std::string("b")));
  EXPECT_TRUE(bf.Contains(std::string("c")));
  EXPECT_EQ(3, bf.Size());
}

/**
 * 配列サイズ指定で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, NormalConstructor1) {
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf(8);
  bf.Insert(std::string("a"));
  bf.Insert(std::string("b"));
  bf.Insert(std::string("c"));
  EXPECT_TRUE(bf.Contains(std::string("a")));
  EXPECT_TRUE(bf.Contains(std::string("b")));
  EXPECT_TRUE(bf.Contains(std::string("c")));
  EXPECT_EQ(3, bf.Size());
}

/**
 * 配列サイズとハッシュ関数の個数指定で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, NormalConstructor2) {
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf(8, 5);
  bf.Insert(std::string("a"));
  bf.Insert(std::string("b"));
  bf.Insert(std::string("c"));
  EXPECT_TRUE(bf.Contains(std::string("a")));
  EXPECT_TRUE(bf.Contains(std::string("b")));
  EXPECT_TRUE(bf.Contains(std::string("c")));
  EXPECT_EQ(3, bf.Size());
}

/**
 * int型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, IntBloomFilter) {
  using element_type_t = int;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1;
  element_type_t b = 2;
  element_type_t c = 3;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * unsigned int型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, UnsignedIntBloomFilter) {
  using element_type_t = unsigned int;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1;
  element_type_t b = 2;
  element_type_t c = 3;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * long型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, LongBloomFilter) {
  using element_type_t = long;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1;
  element_type_t b = 2;
  element_type_t c = 3;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * unsigned long型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, UnsignedLongBloomFilter) {
  using element_type_t = unsigned long;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1;
  element_type_t b = 2;
  element_type_t c = 3;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * long long型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, LongLongBloomFilter) {
  using element_type_t = long long;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1;
  element_type_t b = 2;
  element_type_t c = 3;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * unsigned long long型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, UnsignedLongLongBloomFilter) {
  using element_type_t = unsigned long long;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1;
  element_type_t b = 2;
  element_type_t c = 3;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * float型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, FloatBloomFilter) {
  using element_type_t = float;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1.1;
  element_type_t b = 2.1;
  element_type_t c = 3.1;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * double型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, DoubleBloomFilter) {
  using element_type_t = double;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1.1;
  element_type_t b = 2.1;
  element_type_t c = 3.1;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * long double型で要素の追加・判定・サイズ取得ができることを確認する．
 */
TEST_F(BloomFilterTest, LongDoubleBloomFilter) {
  using element_type_t = long double;
  using bf_t = sbf::BloomFilter<element_type_t>;
  bf_t bf;
  element_type_t a = 1.1;
  element_type_t b = 2.1;
  element_type_t c = 3.1;
  bf.Insert(a);
  bf.Insert(b);
  bf.Insert(c);
  EXPECT_TRUE(bf.Contains(a));
  EXPECT_TRUE(bf.Contains(b));
  EXPECT_TRUE(bf.Contains(c));
  EXPECT_EQ(3, bf.Size());
}

/**
 * 集合に含まれない値について含まれないと判定されることを確認する．
 */
TEST_F(BloomFilterTest, NotContained) {
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf;
  bf.Insert(std::string("a"));
  bf.Insert(std::string("b"));
  bf.Insert(std::string("c"));
  EXPECT_FALSE(bf.Contains(std::string("d")));
  EXPECT_FALSE(bf.Contains(std::string("e")));
  EXPECT_FALSE(bf.Contains(std::string("f")));
}

/**
 * 配列サイズが不正の場合にエラーとわかることを確認する．
 */
TEST_F(BloomFilterTest, ErrorNumBits) {
  using bf_t = sbf::BloomFilter<std::string>;
  try {
    bf_t bf(100);
    EXPECT_TRUE(bf.HasParameterError());
    auto flags = bf.ParameterErrorFlags();
    EXPECT_TRUE((flags & bf_t::kHasLog2NumBitsError) != 0);
    bf.ClearParameterError(bf_t::kHasLog2NumBitsError);
    flags = bf.ParameterErrorFlags();
    EXPECT_FALSE(bf.HasParameterError());
    EXPECT_TRUE((flags & bf_t::kHasLog2NumBitsError) == 0);
  }
  catch (const std::bad_alloc& e) {
    // bad_alloc の場合は試験無効だが失敗としない．
  }
  catch (...) {
    FAIL();
  }
}

/**
 * ハッシュ関数の個数が不正の場合にエラーとわかることを確認する．
 */
TEST_F(BloomFilterTest, ErrorNumHashes) {
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf(2, 0);
  EXPECT_TRUE(bf.HasParameterError());
  auto flags = bf.ParameterErrorFlags();
  EXPECT_TRUE((flags & bf_t::kHasNumHashesError) != 0);
  bf.ClearParameterError(bf_t::kHasNumHashesError);
  EXPECT_FALSE(bf.HasParameterError());
  flags = bf.ParameterErrorFlags();
  EXPECT_TRUE((flags & bf_t::kHasNumHashesError) == 0);
  bf.SetNumHashes(0);
  flags = bf.ParameterErrorFlags();
  EXPECT_TRUE((flags & bf_t::kHasNumHashesError) != 0);
  bf.SetNumHashes(2);
  EXPECT_FALSE(bf.HasParameterError());
  flags = bf.ParameterErrorFlags();
  EXPECT_TRUE((flags & bf_t::kHasNumHashesError) == 0);
  bf.SetNumHashes(0);
  flags = bf.ParameterErrorFlags();
  EXPECT_TRUE((flags & bf_t::kHasNumHashesError) != 0);
  bf.ClearParameterError();
  EXPECT_FALSE(bf.HasParameterError());
  flags = bf.ParameterErrorFlags();
  EXPECT_TRUE((flags & bf_t::kHasNumHashesError) == 0);
}

/**
 * ハッシュ関数の最適値設定ができることを確認する．
 */
TEST_F(BloomFilterTest, OptimalNumHashes) {
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf(2);
  EXPECT_TRUE(bf.SetOptimalNumHashes(2));
  EXPECT_FALSE(bf.HasParameterError());
  EXPECT_FALSE(bf.SetOptimalNumHashes(8192));
  EXPECT_FALSE(bf.HasParameterError());
}

} // namespace


