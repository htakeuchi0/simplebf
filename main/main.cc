/**
 * @file main.cc
 * @brief メインメソッドをもつソースファイル．
 */

#include "simplebf/util.h"
#include "simplebf/bloom_filter.h"
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <random>

namespace {

/**
 * テスト用の集合をランダムに生成する．
 *
 * @param[in] size 要素数
 * @param[in,out] rnd 乱数生成器
 * @return 指定された要素数の集合
 */
std::set<std::string> GenerateTestSet(std::size_t size, std::mt19937_64& rnd) {
  std::set<std::string> s;
  for (std::size_t i = 0; i < size; i++) {
    std::string entry;
    while (true) {
      entry = std::to_string(rnd());
      if (s.count(entry) == 0) {
        break;
      }
    }
    s.insert(entry);
  }

  return s;
}

/**
 * 指定された集合と共通部分をもたないテスト用の集合をランダムに生成する．
 *
 * @param[in] size 要素数
 * @param[in] target_set 集合
 * @param[in,out] rnd 乱数生成器
 * @return 指定された要素数の集合
 */
std::set<std::string> GenerateTestSet(std::size_t size,
    const std::set<std::string>& target_set, std::mt19937_64& rnd) {
  std::set<std::string> s;
  for (std::size_t i = 0; i < size; i++) {
    std::string entry;
    while (true) {
      entry = std::to_string(rnd());
      if (s.count(entry) == 0 && target_set.count(entry) == 0) {
        break;
      }
    }
    s.insert(entry);
  }

  return s;
}

/**
 * 文字列集合内のデータサイズの合計を返す．
 *
 * @param[in] set 文字列集合
 * @return データサイズ [bits]
 */
std::size_t TotalSize(const std::set<std::string>& set) {
  std::size_t total_size = 0;
  for (auto&& str : set) {
    // null 文字 (\0) も合わせたビット数を追加する．
    total_size += (str.size() + 1) * 8;
  }

  return total_size;
}

/**
 * 見積もられた偽陽性率を返す．
 *
 * @param[in] num_bits Bloom filter の配列のビット数
 * @param[in] num_hashes ハッシュ関数の個数
 * @param[in] num_entries 追加された要素数
 */
double EstimatedFalsePositiveRatio(std::size_t num_bits, std::size_t num_hashes,
    std::size_t num_entries) {
  std::size_t m = num_bits;
  std::size_t k = num_hashes;
  std::size_t n = num_entries;
  double ratio = std::exp(k*std::log(1 - std::exp(k*n*std::log(1 - 1.0/m))));
  return ratio;
}

/**
 * ヘルプを表示する．
 *
 * @param[in] path この実行ファイルへのパス
 * @param[in] out 出力ストリーム（省略時 std::cout）
 * @return 出力ストリームへの参照
 */
std::ostream& ShowHelp(const std::string& path, std::ostream& out = std::cout) {
  out << "Bloom filter のサンプル実装プログラム．\n";
  out << "\n";
  out << "Usage:\n";
  out << "  " << path << " [log2_num_bits] [num_entries] [num_challenges] [seed]\n";
  out << "  " << path << " --help\n";
  out << "\n";
  out << "Arguments:\n";
  out << "  log2_num_bits: Bloom filter 用配列のビット数の底2による対数値（省略時13）\n";
  out << "  num_entries: 追加する要素数（省略時1024）\n";
  out << "  num_challenges: 偽陽性のテストに使う要素数（省略時1024）\n";
  out << "  seed: 乱数シード（省略時はシード値を乱数で指定）\n";
  out << "\n";
  out << "Examples:\n";
  out << "  " << path << "\n";
  out << "  " << path << " 15\n";
  out << "  " << path << " 15 4096\n";
  out << "  " << path << " 15 4096 1000000\n";
  out << "  " << path << " 15 4096 1000000 1234\n";
  return out;
}

/**
 * コマンドライン引数を解析してパラメータを返す．
 *
 * @param[in] argc コマンドライン引数の数
 * @param[in] argv コマンドライン引数
 * @param[out] log2_num_bits Bloom filter 要配列のビット数の底2による対数値
 * @param[out] num_entries 追加する要素数
 * @param[out] num_challenges 偽陽性テストに使う要素数
 * @return この関数の実行後にただちに終了する場合は true
 */
bool ParseArguments(int argc, char **argv, std::size_t& log2_num_bits,
    std::size_t& num_entries, std::size_t& num_challenges,
    std::optional<std::uint32_t>& seed) {
  if (argc > 1) {
    if (std::string(argv[1]) == "--help" || 
        std::string(argv[1]) == "-h") {
      ShowHelp(std::string(argv[0]));
      return true;
    }
  }

  constexpr std::size_t kDefaultLog2NumBits = 13;
  log2_num_bits = kDefaultLog2NumBits;
  if (argc > 1) {
    std::size_t value = std::atoi(argv[1]);
    log2_num_bits = value;
  }

  constexpr std::size_t kDefaultNumEntries = 1024;
  num_entries = kDefaultNumEntries;
  if (argc > 2) {
    std::size_t value = std::atoi(argv[2]);
    if (value > 0) {
      num_entries = value;
    }
  }

  constexpr std::size_t kDefaultNumChallenges = 1024;
  num_challenges = kDefaultNumChallenges;
  if (argc > 3) {
    int value = std::atoi(argv[3]);
    if (value > 0) {
      num_challenges = value;
    }
  }

  seed = std::nullopt;
  if (argc > 4) {
    seed = std::atoi(argv[4]);
  }

  return false;
}

} // namespace

/**
 * メインメソッド
 *
 * @param[in] argc コマンドライン引数の数
 * @param[in] argv コマンドライン引数
 * @return int 終了コード
 */
int main(int argc, char **argv) {
  // コマンドライン引数の解析
  std::size_t log2_num_bits;
  std::size_t num_entries;
  std::size_t num_challenges;
  std::optional<std::uint32_t> seed; 
  bool should_exit = ParseArguments(argc, argv, log2_num_bits,
    num_entries, num_challenges, seed);
  if (should_exit) {
    return 0;
  }

  // 乱数生成器の生成
  std::mt19937_64 rnd(seed ? seed.value() : std::random_device{}());

  // 配列サイズを設定して Bloom filter クラスを初期化．
  using bf_t = sbf::BloomFilter<std::string>;
  bf_t bf(log2_num_bits);
  if (bf.HasParameterError()) {
    if ((bf.ParameterErrorFlags() & bf_t::kHasLog2NumBitsError)
        != 0) {
      std::cerr << "Failed to set the size of filter list." << std::endl;
      return 1;
    }
  }

  // テスト集合を取得
  const auto& test_set = GenerateTestSet(num_entries, rnd);
  std::size_t total_size = TotalSize(test_set);

  // テスト設定を出力
  std::cout << "[Test setting]\n";
  std::cout << "The number of entries         : " << num_entries << "\n";
  std::cout << "The total data size           : " << total_size << " [bits]\n";
  std::cout << "\n";

  // 要素数の最大値から最適なハッシュ関数の個数を設定．
  bool successful = bf.SetOptimalNumHashes(num_entries);
  if (!successful) {
    std::cerr << "Warning: Failed to set optimal number of hash functions" << std::endl;
  }

  // Bloom filter 設定を出力
  std::cout << "[Bloom filter setting]\n";
  std::cout << "The filter size               : " << bf.NumBits() << " [bits]\n";
  std::cout << "The number of hash functions  : " << bf.NumHashes() << "\n";
  std::cout << "\n";

  // Bloom filter に要素を追加
  for (auto&& entry : test_set) {
    bf.Insert(entry);
  }

  // テスト結果を出力
  std::cout << "[Bloom filter test]\n";

  // True Positive の計算
  std::size_t count = 0;
  for (auto&& entry : test_set) {
    if (bf.Contains(entry)) {
      count++;
    }
  }
  double true_positive_ratio = static_cast<double>(count) / num_entries;
  std::cout << "True Positive Rate            : " << true_positive_ratio << std::endl;

  double estimated_tp = 1.0;
  std::cout << "Estimated True Positive Rate  : " << estimated_tp << std::endl;

  // False Positive の計算
  std::size_t contains = 0;
  const auto& challenge_set = GenerateTestSet(num_challenges, test_set, rnd);
  for (auto&& entry : challenge_set) {
    if (bf.Contains(entry)) {
      contains++;
    }
  }
  double false_positive_ratio = static_cast<double>(contains) / num_challenges;
  std::cout << "False Positive Rate           : " << false_positive_ratio << std::endl;

  double estimated_fp = EstimatedFalsePositiveRatio(bf.NumBits(),
    bf.NumHashes(), num_entries);

  std::cout << "Estimated False Positive Rate : " << estimated_fp << std::endl;

  return 0;
}
