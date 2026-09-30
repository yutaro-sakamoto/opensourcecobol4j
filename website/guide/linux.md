# Install on Linux

Hagane COBOL is tested on the following platforms:

| OS | JDK |
| -- | -- |
| Ubuntu 26.04 | OpenJDK 21 |
| AlmaLinux 9 | OpenJDK 11 |

## 1. Install the dependencies

::: code-group

```bash [Ubuntu]
sudo apt-get update
sudo apt-get install -y default-jdk build-essential bison flex gettext texinfo libgmp-dev autoconf
```

```bash [AlmaLinux]
sudo dnf -y update
sudo dnf install -y java-11-openjdk-devel gcc make bison flex automake autoconf diffutils gettext
```

:::

## 2. Build and install Hagane COBOL

Download the source code of the [latest release](https://github.com/yutaro-sakamoto/Hagane-COBOL/releases), then build and install it.

::: code-group

```bash [Shift_JIS source code (default)]
curl -L -o hagane-cobol-v1.0.0.tar.gz https://github.com/yutaro-sakamoto/Hagane-COBOL/archive/refs/tags/v1.0.0.tar.gz
tar zxvf hagane-cobol-v1.0.0.tar.gz
cd Hagane-COBOL-1.0.0
./configure --prefix=/usr/
make
sudo make install
```

```bash [UTF-8 source code]
curl -L -o hagane-cobol-v1.0.0.tar.gz https://github.com/yutaro-sakamoto/Hagane-COBOL/archive/refs/tags/v1.0.0.tar.gz
tar zxvf hagane-cobol-v1.0.0.tar.gz
cd Hagane-COBOL-1.0.0
./configure --prefix=/usr/ --enable-utf8
touch cobj/*.m4
make
sudo make install
```

:::

## 3. Set CLASSPATH

Add `libcobj.jar` to the `CLASSPATH` environment variable, for example in `~/.bashrc`:

```bash
export CLASSPATH="$CLASSPATH:/usr/lib/opensourcecobol4j/libcobj.jar"
```

## 4. Check the installation

```bash
cobj --version
```

Next, see [Usage](./usage) to compile and run your first program.
