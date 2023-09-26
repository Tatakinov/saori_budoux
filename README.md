# How to Build

[jsonライブラリ](https://github.com/nlohmann/json)をダウンロードして
saori\_budoux/include/nlohmann/json.hppに配置してmake

[budoux](https://github.com/google/budoux)のbudoux/models/ja.jsonを
出来上がったdllと同じ場所にmodel.jsonにリネームして配置。

# 使い方

Argument0: 何か整形したい文字列
Argument1: 最大文字列幅(optional)

Result: Valueの数
ValueN: 整形された文字列

文字幅を設定しないとbudouxの出力をそのまま返す。
文字幅を設定するとその幅を超えないように整形する。

## 例

saori("あなたに寄り添う最先端のテクノロジー。")
    => ("4", "あなたに", "寄り添う", "最先端の", "テクノロジー。")

saori("あなたに寄り添う最先端のテクノロジー。", 20)
    => ("3", "あなたに寄り添う", "最先端の", "テクノロジー。")

# 使用ライブラリなど

- [json](https://github.com/nlohmann/json)
- [budoux](https://github.com/google/budoux)
