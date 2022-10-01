/**
 * @file util.cc
 * @brief ユーティリティ関数を定義するソースファイル．
 */

#include "simplebf/util.h"

/**
 * @brief Bloom filter のための名前空間．
 */
namespace sbf {

/**
 * @brief ハッシュ関数のための名前空間．
 */
namespace hash {

/**
 * Daniel J. Bernstein によるハッシュ関数によるハッシュ値を返す．
 *
 * 参考：http://www.cse.yorku.ca/~oz/hash.html
 *
 * @param[in] str 文字列
 * @return ハッシュ値
 */
std::size_t Djb2(const std::string& str) {
  static constexpr std::size_t kHashDjb2 = 5381;
  std::size_t hash = kHashDjb2;
  for (char c : str) {
    // 33*hash + c
    hash = ((hash << 5) + hash) + static_cast<std::size_t>(c);
  }
  return hash;
}

} // namespace hash 

} // namespace sbf
