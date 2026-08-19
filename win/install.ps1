# Install the opensource COBOL 4J Windows bundle and set up the user
# environment variables. Run from the extracted bundle directory:
#
#   powershell -ExecutionPolicy Bypass -File .\install.ps1 [-InstallDir <dir>]
#
# A JDK (11 or later) must be installed separately; cobj runs javac and jar.

param(
    [string]$InstallDir = "C:\opensourcecobol4j"
)

$ErrorActionPreference = "Stop"

foreach ($dir in "bin", "lib", "config", "copy") {
    New-Item -ItemType Directory -Force -Path (Join-Path $InstallDir $dir) | Out-Null
    Copy-Item -Path (Join-Path $PSScriptRoot "$dir\*") -Destination (Join-Path $InstallDir $dir) -Force
}

# Prepend $InstallDir\bin to the user PATH unless it is already there.
$binDir = Join-Path $InstallDir "bin"
$userPath = [Environment]::GetEnvironmentVariable("PATH", "User")
if (-not ($userPath -split ";" -contains $binDir)) {
    $newPath = if ($userPath) { "$binDir;$userPath" } else { $binDir }
    [Environment]::SetEnvironmentVariable("PATH", $newPath, "User")
}

# Make libcobj.jar and the current directory visible to the JVM.
$jar = Join-Path $InstallDir "lib\libcobj.jar"
$userClasspath = [Environment]::GetEnvironmentVariable("CLASSPATH", "User")
if (-not ($userClasspath -split ";" -contains $jar)) {
    $newClasspath = if ($userClasspath) { "$jar;.;$userClasspath" } else { "$jar;." }
    [Environment]::SetEnvironmentVariable("CLASSPATH", $newClasspath, "User")
}

[Environment]::SetEnvironmentVariable("COB_CONFIG_DIR", (Join-Path $InstallDir "config"), "User")
[Environment]::SetEnvironmentVariable("COB_COPY_DIR", (Join-Path $InstallDir "copy"), "User")

Write-Host "Installed opensource COBOL 4J to $InstallDir."
Write-Host "User environment variables PATH, CLASSPATH, COB_CONFIG_DIR and COB_COPY_DIR were updated."
Write-Host "Open a new terminal for the changes to take effect."
Write-Host "A JDK (11 or later) is required to compile and run COBOL programs."
