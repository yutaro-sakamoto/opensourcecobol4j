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

# PATH and CLASSPATH are read and written through the registry so that
# REG_EXPAND_SZ values keep their unexpanded %VARIABLE% references.
# [Environment]::GetEnvironmentVariable would expand them, and writing the
# expanded result back would freeze them to their current values.
$envKey = [Microsoft.Win32.Registry]::CurrentUser.CreateSubKey("Environment")

# Prepend $InstallDir\bin to the user PATH unless it is already there.
$binDir = Join-Path $InstallDir "bin"
$userPath = [string]$envKey.GetValue("PATH", "",
    [Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames)
$pathEntries = $userPath -split ";" | ForEach-Object { $_.TrimEnd("\").Trim('"') }
if (-not ($pathEntries -contains $binDir)) {
    $newPath = if ($userPath) { "$binDir;$userPath" } else { $binDir }
    $envKey.SetValue("PATH", $newPath, [Microsoft.Win32.RegistryValueKind]::ExpandString)
}

# Make libcobj.jar and the current directory visible to the JVM.
$jar = Join-Path $InstallDir "lib\libcobj.jar"
$userClasspath = [string]$envKey.GetValue("CLASSPATH", "",
    [Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames)
$classpathEntries = $userClasspath -split ";" | ForEach-Object { $_.TrimEnd("\").Trim('"') }
if (-not ($classpathEntries -contains $jar)) {
    $newClasspath = if ($userClasspath) { "$jar;.;$userClasspath" } else { "$jar;." }
    $envKey.SetValue("CLASSPATH", $newClasspath, [Microsoft.Win32.RegistryValueKind]::ExpandString)
}

$envKey.Close()

# Set through [Environment] so that a WM_SETTINGCHANGE broadcast tells the
# shell to reload the environment, covering the registry writes above too.
foreach ($pair in @(
        @{ Name = "COB_CONFIG_DIR"; Value = (Join-Path $InstallDir "config") },
        @{ Name = "COB_COPY_DIR"; Value = (Join-Path $InstallDir "copy") })) {
    $old = [Environment]::GetEnvironmentVariable($pair.Name, "User")
    if ($old -and $old -ne $pair.Value) {
        Write-Host "Overwriting $($pair.Name) (was: $old)"
    }
    [Environment]::SetEnvironmentVariable($pair.Name, $pair.Value, "User")
}

Write-Host "Installed opensource COBOL 4J to $InstallDir."
Write-Host "User environment variables PATH, CLASSPATH, COB_CONFIG_DIR and COB_COPY_DIR were updated."
Write-Host "Open a new terminal for the changes to take effect."
Write-Host "A JDK (11 or later) is required to compile and run COBOL programs."
