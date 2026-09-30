# Windowsへのインストール

Windowsでは、Visual Studioで`cobj.exe`を、JDKで`libcobj.jar`をビルドします。
OpenJDK 21でテストされています。

## 1. ツールの準備

1. [Visual Studio](https://visualstudio.microsoft.com/)を、**C++によるデスクトップ開発**のワークロードを含めてインストールします。
2. [JDK](https://adoptium.net/)をインストールします。
3. [最新のリリース](https://github.com/yutaro-sakamoto/Hagane-COBOL/releases)のソースコードをダウンロードして展開します。

## 2. cobj.exeのビルド

1. `win/opensourcecobol4j.sln`をVisual Studioで開きます。
2. 構成で**Release**を選びます。
3. **ビルド** → **ソリューションのビルド**をクリックします。

`win/x64/Release`に`cobj.exe`ができます。

## 3. libcobj.jarのビルド

ソースコードのディレクトリでPowerShellを開き、次を実行します。

```powershell
cd libcobj
.\gradlew shadowJar
```

`libcobj\app\build\libs\`に`libcobj.jar`ができます。

## 4. ファイルの配置

`win`ディレクトリで`make-install.ps1`を実行します。

```powershell
cd win
.\make-install.ps1
```

各ファイルは次の場所に配置されます。

| ファイル | 配置先 |
| -- | -- |
| cobj.exe | `C:\opensourcecobol4j\bin` |
| libcobj.jar | `C:\opensourcecobol4j\lib` |
| configファイル | `C:\opensourcecobol4j\config` |
| copyファイル | `C:\opensourcecobol4j\copy` |

::: tip
Debug構成でビルドした場合は、`win/make-install.ps1`の`\x64\Release\cobj.exe`を`\x64\Debug\cobj.exe`に書き換えてください。
:::

## 5. 環境変数の設定

1. `C:\opensourcecobol4j\bin`を環境変数`PATH`に追加します。
2. 環境変数`CLASSPATH`を`.;C:\opensourcecobol4j\lib\libcobj.jar`にします。カレントディレクトリのプログラムを実行するため、先頭の`.`が必要です。

新しいターミナルを開いて、インストールを確認します。

```powershell
cobj --version
```

最初のプログラムのコンパイルと実行は、[使い方](./usage)をご覧ください。
