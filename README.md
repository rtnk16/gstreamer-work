# gstreamer-work

GStreamer でカラーバー画像（640×360）を作る小さな C アプリです。
画面・カメラ・音声デバイスは不要です。

## 実行

このクラウド環境では、そのまま実行できます。毎回コンパイルして起動します。

```bash
cd /workspace/gstreamer-work
bash run.sh
```

`snapshot.png` に画像を保存します。出力先の指定もできます。

```bash
bash run.sh /tmp/test-image.png
```

パイプライン: `videotestsrc → videoconvert → pngenc → filesink`。
エラーや10秒のタイムアウト時には、終了コード1を返します。

別の Debian / Ubuntu 環境では C コンパイラ、pkg-config、
`libgstreamer1.0-dev`、`gstreamer1.0-plugins-base`、
`gstreamer1.0-plugins-good` が必要です。
