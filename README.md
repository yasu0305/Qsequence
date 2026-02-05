# Qsequence

Qt6/C++17による逐次処理・進捗表示GUIアプリ

## 概要
- A→Dの複数プロセスを順次実行し、進捗をプログレスバーで表示
- 各プロセスは状態マシンで管理（今後スキップ・リトライ拡張も容易）
- Stopボタンで安全に中断・UIロック、終了後にリセット
- 日付時刻表示（右上固定・1秒ごと自動更新）
- UIは英語表記

## ビルド方法

### 必要環境
- Linux（apt版Qt6推奨）
- Qt6 (qt6-base-dev, qt6-tools-dev など)
- g++ (C++17対応)
- qmake

### ビルド手順
```sh
mkdir build
cd build
qmake ..
make -j4
```

## 実行方法
```sh
./Qsequence
```

## 主なファイル
- mainwindow.h / mainwindow.cpp : メインウィンドウ・UI制御
- worker.h / worker.cpp : 状態マシン・処理ロジック
- main.cpp : アプリ起動エントリ
- Qsequence.pro : qmakeビルド設定
- .clang-format : Qt/LLVM系C++整形ルール

## 機能
- Start: 処理開始（A→Dを順次実行）
- Stop: 実行中断（UIロック→完全停止後リセット）
- Progress Bar: 各プロセス進捗を自動計算で表示
- DateTime: 現在日時を右上に常時表示

## カスタマイズ
- 各プロセスの処理内容はworker.cppのprocessA/B/C/D内に記述
- プロセス数を増減する場合はworker.hのState列挙体・stateList/stateCountを編集

## 注意
- プログレスバーの%はプロセス数から自動計算されます
- .clang-formatはQt/LLVM系で統一
- QStandardPaths警告はchmod 700 /run/user/1000/で解消可

---

ご質問・機能追加要望はお気軽にどうぞ。
