# asfxc

Shader Variation Generator.

## About
 シェーダエフェクトファイル(*.fx)を基に，シェーダバリエーションを生成するツールです.  
 ラスタライザーステートやブレンドステート，深度ステンシルステートの記述に対応しています.  
 
## How to use?
コマンドラインからの使用は次の通りです.  

```
asfxc.exe input_path -o output_dir [-c]
```

第1引数は，入力となるシェーダエフェクトファイル(*.fx)のファイルパスを指定します.  
第2引数は，出力先となるディレクトリパスを指定します.  
オプション引数「-c」で，シェーダコンパイルを行い，バイナリ出力を行います.  

 
## License
 This software is distributed under MIT. For details, see LICENCE file.  

