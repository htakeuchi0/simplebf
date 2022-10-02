# C++ による Bloom filter のサンプル実装

## 概要

本ソースセットは，C++ による Bloom filter のサンプル実装です．

確認済みの動作環境は以下のとおりです．    
ただし，単体テストを除き，標準ライブラリのみしか使われていないので，    
ほとんどの g++ 環境で実行可能のはずです．   
* OS: Linux (Ubuntu 18.04.5 LTS)
  * コンパイラ: g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

以下の記述に基づく簡易的な実装としています．    
特に，実行可能ファイルのビルドだけであれば，標準ライブラリのみでビルドできるようにしています．    
C++ のバージョンは C++17 とします．
* https://en.wikipedia.org/wiki/Bloom_filter

フィルタには `std::vector<bool>` を利用します．    
`std::vector` は `bool` 型に対して特殊化されており，各要素は1ビットの領域のみが必要となるように最適化されています．

ハッシュ関数は，`std::hash` と djb2 を用いた enhanced double hashing で生成します．    
ただし，数値型の場合は，`std::to_string` で文字列化したものを djb2 に通します．    
より正確には，double hashing における2個目のハッシュ値は，djb2 で得たハッシュ値を2倍して1を足したものを利用し，各ハッシュ値はフィルタサイズで割った余りを使って，enhanced double hashing で複数のハッシュ値を得ます．

`main/main.cc` に，文字列集合に対する Bloom filter を作成し，true positive rate と false positive rate を計算するサンプル実装があります．

実行例は以下のとおりです．

```
$ ./bf 
[Test setting]
The number of entries         : 1024
The total data size           : 166720 [bits]

[Bloom filter setting]
The filter size               : 8192 [bits]
The number of hash functions  : 5

[Bloom filter test]
True Positive Rate            : 1
Estimated True Positive Rate  : 1
False Positive Rate           : 0.0234375
Estimated False Positive Rate : 0.021684
```

`total data size` は各文字列のサイズから計算した，文字列集合の大きさの概算値を表します．    
`filter size` は Bloom filter の大きさで，もとの集合の大きさに依存しないサイズです．
ハッシュ関数の個数は，集合の要素数とフィルタサイズから，偽陽性率が最小になることが期待される値を設定しています．

このとき，true positive rate は 1, false positive rate は 0.0234375 となっています．    
それぞれ，理論値におおよそ近い値になっていることがわかります．

いくつかの実験パラメータはコマンドライン引数から与えられます．    
詳細は `./bf --help` を参照して下さい．

## 準備

本ソースセットでは，単体試験，カバレッジ計測，ドキュメント生成に，それぞれ以下を利用します．
* Google Test
* LCOV
* doxygen

`scripts` ディレクトリにそれぞれをインストールするためのスクリプトが配置されているため，
必要に応じて実行してください．

実行可能ファイルのビルドだけであれば，`make`, `g++` があれば十分です．

## 使い方

### サンプルコードの実行

ソースコードをビルドしてサンプルコードを実行する方法は以下のとおりです．
`make` は `make build` でも可能です．

```
$ make
$ ./bf
```

### 共有ライブラリのビルドとインストール

ソースコードをビルドして共有ライブラリを生成する方法は以下のとおりです．

```
$ make lib
```

ヘッダファイルと共有ライブラリをインストールする方法は以下のとおりです．   
`/usr/local/include/simplebf/` 以下にヘッダファイルが配置され，`/usr/local/lib/` 以下に共有ライブラリ `libsimplebf.so`, `libsimplebf.so.*` が配置されます．

```
$ make lib
$ sudo make install
```

別プロジェクトでこの共有ライブラリを使う場合は，`libsimplebf.so` をリンクして下さい (`-lsimplebf`)．

ヘッダファイルと共有ライブラリをアンインストールする方法は以下のとおりです．

```
$ sudo make uninstall
```

### テストコードの実行

テストコードをビルドしてテストを実行する方法は以下のとおりです．    
これを実行するには Google Test のインストールが必要です．

```
$ make test
$ ./test_bf
```

### テストコードとカバレッジツールの実行

テストコードをビルドしてテストを実行し，カバレッジ計測結果を出力する方法は以下のとおりです．    
これを実行するには LCOV のインストールが必要です．

```
$ make lcov
```

結果はブラウザから確認できます．

```
$ firefox lcov/index.html &
```

### ドキュメントの生成

ドキュメントを生成する方法は以下のとおりです．    
これを実行するには doxygen のインストールが必要です．

```
$ make docs
```

結果はブラウザから確認できます．

```
$ firefox doxygen/html/index.html &
```

### 一括実行

ソースコード，共有ライブラリ，テストコードをビルドし，カバレッジ計測結果とドキュメント生成まで行う方法は以下のとおりです．

```
$ make all
```
