/**
 * @file util.h
 * @brief ユーティリティ関数を宣言するヘッダファイル．
 */

#ifndef CPPBF_UTIL_H_
#define CPPBF_UTIL_H_

#include <cstddef>
#include <string>

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
std::size_t Djb2(const std::string& str);

} // namespace hash 

} // namespace sbf

#endif // #ifndef CPPBF_UTIL_H_
