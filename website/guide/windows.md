# Install on Windows

Hagane COBOL is built with Visual Studio (for `cobj.exe`) and a JDK (for `libcobj.jar`).
It is tested with OpenJDK 21.

## 1. Prepare the tools

1. Install [Visual Studio](https://visualstudio.microsoft.com/) with the **Desktop development with C++** workload.
2. Install a [JDK](https://adoptium.net/).
3. Download the source code of the [latest release](https://github.com/yutaro-sakamoto/Hagane-COBOL/releases) and extract it.

## 2. Build cobj.exe

1. Open `win/opensourcecobol4j.sln` with Visual Studio.
2. Select the **Release** configuration.
3. Click **Build** → **Build Solution**.

`cobj.exe` is created in `win/x64/Release`.

## 3. Build libcobj.jar

Open PowerShell in the source directory and run:

```powershell
cd libcobj
.\gradlew shadowJar
```

`libcobj.jar` is created in `libcobj\app\build\libs\`.

## 4. Install the files

Run `make-install.ps1` in the `win` directory:

```powershell
cd win
.\make-install.ps1
```

The files are placed as follows:

| File | Location |
| -- | -- |
| cobj.exe | `C:\opensourcecobol4j\bin` |
| libcobj.jar | `C:\opensourcecobol4j\lib` |
| config files | `C:\opensourcecobol4j\config` |
| copy files | `C:\opensourcecobol4j\copy` |

::: tip
If you built the Debug configuration, change `\x64\Release\cobj.exe` in `win/make-install.ps1` to `\x64\Debug\cobj.exe`.
:::

## 5. Set the environment variables

1. Add `C:\opensourcecobol4j\bin` to `PATH`.
2. Set `CLASSPATH` to `.;C:\opensourcecobol4j\lib\libcobj.jar`. The leading `.` is needed to run the programs in the current directory.

Open a new terminal and check the installation:

```powershell
cobj --version
```

Next, see [Usage](./usage) to compile and run your first program.
