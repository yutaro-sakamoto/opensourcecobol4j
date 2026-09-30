# Dockerで動かす

Hagane COBOLをインストールしたDockerイメージを
[GitHub Container Registry](https://github.com/yutaro-sakamoto/Hagane-COBOL/pkgs/container/hagane-cobol)で配布しています。

| イメージ | ソースコードの文字コード |
| -- | -- |
| `ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0` | Shift_JIS |
| `ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0-utf8` | UTF-8 |

## サンプルプログラムを動かす

```bash
docker run -it --rm ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0
```

コンテナの中で次を実行します。

```bash
cd /root/cobol_sample
cobj HELLO.cbl
java HELLO
# HELLO WORLD!
```

## 手元のプログラムをコンパイルする

カレントディレクトリをコンテナにマウントして、その中のプログラムをコンパイルします。

```bash
docker run --rm -v "$PWD":/work -w /work ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0 \
  bash -c 'cobj HELLO.cbl && java HELLO'
```

WindowsのPowerShellでは、`"$PWD"`の代わりに`${PWD}`と書きます。

::: info
コンテナはrootで動くため、Linuxのホストでは生成された`.java`と`.class`ファイルの所有者がrootになります。
:::
