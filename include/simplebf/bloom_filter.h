/**
 * @file bloom_filter.h
 * @brief Bloom filter 用クラスを宣言するヘッダファイル．
 */

#ifndef CPPBF_BLOOM_FILTER_H_
#define CPPBF_BLOOM_FILTER_H_

#include "util.h"
#include <cmath>
#include <cstddef>
#include <vector>
#include <functional>

/**
 * @brief Bloom filter のための名前空間．
 */
namespace sbf {

/**
 * @brief Bloom filter 用のクラス．
 *
 * 要素が含まれるかを確率的に判定できる空間効率のよいデータ構造．<br>
 * 要素の追加と要素が含まれているかの判定ができる．<br>
 * 要素が含まれているかの判定は確率的であり，具体的には以下のとおりである．
 * * 含まれているのに含まれていないと判定される誤り (false negative) は発生しない
 * * 含まれていないのに含まれていると判定される誤り (false positive) が発生することがある
 *
 * @tparam T 要素の型
 */
template <class T>
class BloomFilter {
  // std::to_string 可能な型か文字列型のみを認める．
  static_assert(std::is_same<T, int>::value ||
    std::is_same<T, unsigned int>::value ||
    std::is_same<T, long>::value ||
    std::is_same<T, unsigned long>::value ||
    std::is_same<T, long long>::value ||
    std::is_same<T, unsigned long long>::value ||
    std::is_same<T, float>::value ||
    std::is_same<T, double>::value ||
    std::is_same<T, long double>::value ||
    std::is_same<T, std::string>::value,
      "This class template only supports std::string or "
      "types that can be converted to std::string by std::to_string.");

public:
  /** デフォルトコンストラクタ． */
  BloomFilter() : BloomFilter(kDefaultLog2NumBits) {
  }

  /**
   * フィルタ用配列サイズのビット数を与えて初期化する．
   *
   * @param[in] num_bits フィルタ用配列サイズのビット数．
   */
  BloomFilter(std::size_t num_bits) : BloomFilter(num_bits, kNumDefaultNumHashes) {
  }

  /**
   * フィルタ用配列サイズのビット数とハッシュ関数の個数を与えて初期化する．
   *
   * @param[in] num_bits フィルタ用配列サイズのビット数．
   * @param[in] num_hashes ハッシュ関数の個数．
   */
  BloomFilter(std::size_t num_bits, std::size_t num_hashes) : size_(0),
      parameter_error_flags_(0) {
    SetLog2NumBits(num_bits);
    SetNumHashes(num_hashes);
  }

  /**
   * 要素を追加する．
   *
   * @param[in] entry 追加する要素
   */
  void Insert(const T& entry) {
    const auto& hashes = Hash(entry);
    for (auto&& hash : hashes) {
      filter_[hash] = true;
    }
    size_++;
  }

  /**
   * 要素が含まれているかを確率的に判定する．
   *
   * 確率的な判定なので誤る可能性がある．具体的には以下のとおりである．
   * * 含まれているのに含まれていないと判定される誤り (false negative) は発生しない
   * * 含まれていないのに含まれていると判定される誤り (false positive) が発生することがある
   *
   * @param[in] entry 要素が含まれているかを判定したい要素
   * @return ハッシュ値
   */
  bool Contains(const T& entry) const {
    const auto& hashes = Hash(entry);
    for (auto&& hash : hashes) {
      if (!filter_[hash]) {
        return false;
      }
    }
    return true; 
  }

  /**
   * double hashing 向けのハッシュ値を返す． 
   *
   * 0以上 NumBits() 未満の値を返す．<br>
   * より具体的には，std::hash によるハッシュ値を NumBits() で割った余りを返す．
   *
   * @param[in] entry ハッシュ値を計算したい要素
   * @return ハッシュ値
   */
  std::size_t FirstHash(const T& entry) const {
    std::size_t hash = ModNumBits(std::hash<T>{}(entry));
    return hash;
  }

  /**
   * double hashing 向けのハッシュ値を返す．
   *
   * 1以上 NumBits() 未満であって， NumBits() と互いに素な値を返す．<br>
   * NumBits() は2べきなので，FirstHash() とは異なるハッシュ関数による値を
   * 2倍して1を足したものを返す．
   * 
   * より具体的には，入力値を文字列化したものの djb2 によるハッシュ値を計算し，<br>
   * その値を2倍して1を足したものを， NumBits() で割った余りを返す．
   *
   * @param[in] entry ハッシュ値を計算したい要素
   * @return ハッシュ値
   */
  std::size_t SecondHash(const T& entry) const {
    std::size_t hash = ModNumBits((hash::Djb2(std::to_string(entry)) << 1) | 1);
    return hash;
  }

  /**
   * 複数のハッシュ関数のハッシュ値を返す．
   *
   * Enhanced double hashing によるハッシュ値を返す．<br>
   * 具体的には，2種類のハッシュ関数 h1, h2 について，<br>
   * hashes[i] = h1(entry) + i*h2(entry) + (i*i*i - i)/6 を返す．<br>
   * 参考：https://www.khoury.northeastern.edu/~pete/pub/bloom-filters-verification.pdf
   *
   * @param[in] entry ハッシュ値を計算したい要素
   * @return 複数のハッシュ値を並べたベクトル
   */
  std::vector<std::size_t> Hash(const T& entry) const {
    std::size_t a = FirstHash(entry);
    std::size_t b = SecondHash(entry);
    std::size_t num_hashes = NumHashes();
    std::vector<std::size_t> hashes(num_hashes);
    hashes[0] = a;
    std::size_t mask = NumBits() - 1;
    for (std::size_t i = 1; i < num_hashes; i++) {
      a = (a + b) & mask;
      b = (b + i) & mask;
      hashes[i] = a;
    }
    return hashes;
  }

  /**
   * フィルタ用配列サイズのビット数を返す．
   *
   * @return フィルタ用配列サイズのビット数
   */
  std::size_t NumBits() const {
    return filter_.size();
  }

  /**
   * フィルタ用配列サイズのビット数の底2による対数値を設定する．
   *
   * 指定値が設定できたら true を返す．<br>
   * フィルタ用配列は1GB程度を限度とし，指定値が34以上の場合は33を設定し，false を返す．<br>
   *
   * @param[in] log2_num_bits フィルタ用配列サイズのビット数の底2による対数値
   * @return 指定値が設定できたら true
   */
  bool SetLog2NumBits(std::size_t log2_num_bits) {
    if (log2_num_bits > 33) {
      // 2^33 [bits] = 2^3 * 2^30 [bits] = 8 * (2^10)^3 [bits] = 1 [GB]
      filter_.resize(1ull << 33);
      parameter_error_flags_ |= kHasLog2NumBitsError;
      return false;
    }

    filter_.resize(1ull << log2_num_bits);
    ClearParameterError(kHasLog2NumBitsError);
    return true;
  }

  /**
   * Bloom filter におけるハッシュ関数の個数を返す．
   *
   * @return Bloom filter におけるハッシュ関数の個数
   */
  std::size_t NumHashes() const {
    return num_hashes_;
  }

  /**
   * Bloom filter におけるハッシュ関数の個数を設定する．
   *
   * 指定値が設定できたら true を返す．<br>
   * 指定値が1未満の場合は1を設定し，false を返す．
   *
   * @param[in] num_hashes Bloom filter におけるハッシュ関数の個数
   * @return 指定値が設定できたら true
   */
  bool SetNumHashes(std::size_t num_hashes) {
    if (num_hashes < 1) {
      num_hashes_ = 1;
      parameter_error_flags_ |= kHasNumHashesError;
      return false;
    }

    num_hashes_ = num_hashes;
    ClearParameterError(kHasNumHashesError);
    return true;
  }

  /**
   * Bloom filter におけるハッシュ関数の個数を最適値で設定する．
   *
   * フィルタ用配列サイズと追加される要素数の最大値を固定した場合に，
   * 偽陽性率 (false positive) が最小となるハッシュ関数の個数を返す．<br>
   * ただし，各ビットにフラグが立つ事象を独立である仮定をおいているため，厳密ではない．<br>
   * 最適値が設定できたら true を返す．<br>
   * 最適値が1未満の場合は1を設定し，false を返すが，内部状態的にはエラー扱いにしない．
   *
   * @param[in] max_num_entries 追加される要素数の最大値
   * @return 最適値が設定できたら true
   */
  bool SetOptimalNumHashes(std::size_t max_num_entries) {
    std::size_t num_hashes
      = static_cast<int>(std::log(2) * NumBits() / max_num_entries);
    bool successful = SetNumHashes(num_hashes);

    // 内部エラーフラグは強制的に解除
    ClearParameterError(kHasNumHashesError);
    return successful;
  }

  /**
   * 追加された要素数を返す．
   *
   * @return 追加された要素数．
   */
  std::size_t Size() const {
    return size_;
  }

  /**
   * パラメータエラーを表すビットフラグを返す．
   *
   * @return パラメータエラーを表すビットフラグ．
   */
  int ParameterErrorFlags() const {
    return parameter_error_flags_;
  }

  /**
   * パラメータエラーフラグを解除する．
   *
   * @param[in] flag パラメータエラーフラグ
   */
  void ClearParameterError(int flag = -1) {
    parameter_error_flags_ &= ~flag;
  }

  /**
   * パラメータエラーがあるかを返す．
   *
   * @return パラメータエラーがある場合はtrue.
   */
  bool HasParameterError() const {
    return (parameter_error_flags_ != 0);
  }

private:
  /**
   * ファイル用配列サイズのビット数で割った余りを返す．
   *
   * 配列サイズは2べきである前提とする．
   *
   * @param[in] x 値
   * @return 入力値をファイル用配列サイズのビット数で割った余り
   */
  std::size_t ModNumBits(std::size_t x) const {
    std::size_t mask = NumBits() - 1;
    return (x & mask); 
  }

public:
  /** フィルタ用配列サイズのビット数の底2による対数値の設定に対するビットフラグ */
  static constexpr int kHasLog2NumBitsError = 0x1;

  /** Bloom filter におけるハッシュ関数の個数に対するビットフラグ */
  static constexpr int kHasNumHashesError = 0x2;

private:
  /** フィルタ用配列サイズのビット数の底2による対数値のデフォルト値． */
  static constexpr std::size_t kDefaultLog2NumBits = 8;

  /** Bloom filter におけるハッシュ関数の個数のデフォルト値． */
  static constexpr std::size_t kNumDefaultNumHashes = 5;

private:
  /**
   * Bloom filter 用フィルタ．
   *
   * std::vector テンプレートクラスは bool 型に対して特殊化されており，
   * 各要素1ビットのみで済むように最適化されている．
   * したがって，ベクトルサイズがNならば，Nビットのみが必要となる．
   */
  std::vector<bool> filter_;

  /** Bloom filter におけるハッシュ関数の個数． */
  std::size_t num_hashes_;

  /** 追加された要素数． */
  std::size_t size_;

  /** パラメータエラーを表すビットフラグ. */
  int parameter_error_flags_;
};

/**
 * double hashing 向けのハッシュ値を返す（FirstHashとは異なるハッシュ値）．
 *
 * 1以上 NumBits() 未満であって，NumBits() と互いに素な値を返す．<br>
 * NumBits() は2べきなので，Firsthash とは異なるハッシュ関数による値を
 * 2倍して1を足したものを返す．
 * 
 * より具体的には，入力値の djb2 によるハッシュ値を計算し，<br>
 * その値を2倍して1を足したものを， NumBits() で割った余りを返す．
 *
 * @param[in] entry ハッシュ値を計算したい要素
 * @return ハッシュ値
 */
template <>
std::size_t BloomFilter<std::string>::SecondHash(const std::string& entry) const {
  std::size_t hash = ModNumBits((hash::Djb2(entry) << 1) | 1);
  return hash;
}

} // namespace sbf

#endif // #ifndef CPPBF_BLOOM_FILTER_H_
