---
layout: home

hero:
  name: Hagane COBOL
  text: COBOLからJavaへのコンパイラ
  tagline: COBOLプログラムをJavaプログラムに変換し、JVMの上で動かします。
  actions:
    - theme: brand
      text: Linux
      link: /ja/guide/linux
    - theme: alt
      text: Windows
      link: /ja/guide/windows
    - theme: alt
      text: Docker
      link: /ja/guide/docker
    - theme: alt
      text: APIリファレンス (Javadoc)
      link: /javadoc/libcobj/index.html
      target: _self

features:
  - icon: 🐧
    title: Linux
    details: UbuntuやAlmaLinuxでリリースのソースコードをビルドし、cobjとlibcobj.jarをインストールします。
    link: /ja/guide/linux
    linkText: Linuxへのインストール
  - icon: 🪟
    title: Windows
    details: Visual Studioでcobj.exeを、Gradleでlibcobj.jarをビルドします。
    link: /ja/guide/windows
    linkText: Windowsへのインストール
  - icon: 🐳
    title: Docker
    details: GitHub Container Registryのイメージを取得して、すぐに試せます。
    link: /ja/guide/docker
    linkText: Dockerで動かす
  - icon: 📚
    title: APIリファレンス
    details: 生成されたJavaプログラムが使うランタイムライブラリlibcobj.jarのJavadocです。
    link: /javadoc/libcobj/index.html
    target: _self
    linkText: Javadocを開く
---

## Hello World

```bash
# HELLO.cblをHELLO.javaに変換し、HELLO.classにコンパイルする
cobj HELLO.cbl

# 実行する
java HELLO
```

詳しくは[使い方](./guide/usage)をご覧ください。

## opensource COBOL 4Jからの名称変更

Hagane COBOLは、これまで[opensource COBOL 4J](https://github.com/opensourcecobol/opensourcecobol4j)のフォークとして開発されてきました。
バージョン1.0.0はopensource COBOL 4J 2.1.0をもとにしています。コマンド名(`cobj`、`cobj-idx`、`cobj-api`)、
ランタイムライブラリ`libcobj.jar`、インストール先は変わりません。
