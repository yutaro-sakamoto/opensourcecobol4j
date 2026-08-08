<#
.SYNOPSIS
    Bring up a throwaway PostgreSQL cluster for the ESQL test suites on Windows.

.DESCRIPTION
    The ESQL autotest suites (tests/esql-basic, tests/esql-sqlca, ...) need a
    PostgreSQL server.  On Linux CI that is a service container; on Windows
    there is none, so this script initialises a cluster of its own from an
    existing PostgreSQL installation and starts it.

    It is used by .github/workflows/windows-test-esql.yml and is meant to work
    the same way on a developer's own Windows machine.  Run it, then run the
    suites from tests/ as usual.

    Two details are not free choices:

    --no-locale is required.  initdb rejects --encoding=UTF8 together with a
    locale that implies a different encoding, and the usual Windows default
    (English_United States.1252) implies WIN1252.  The C locale it gives
    instead is compatible with every encoding.  The suites need UTF-8 because
    they store Japanese text, and lc_messages=C also keeps the PostgreSQL error
    texts that tests/esql-misc.src/logging.at matches in English.

    The default port is 55432, not 5432.  tests/esql-sqlca.src/connect-
    disconnect.at connects to a database named "invalid" with no host part,
    which libcobj turns into a JDBC URL for localhost:5432, and it expects that
    to be refused.  If this cluster listened on 5432 the connection would reach
    it and fail authentication instead, and the test would fail.

    The cluster is created with trust authentication, so anything that can
    reach the port can connect as any role.  initdb leaves listen_addresses at
    localhost, so that is the local machine only.  Do not point this script at
    a cluster that holds anything you care about: it is a test fixture.

.PARAMETER Port
    Port the server listens on.  Must match DB_PORT in
    .github/workflows/db-settings/embed_db_info_windows.sh, which is what tells
    the test programs where to connect.

.PARAMETER DataDir
    Cluster directory.  Defaults to a directory under RUNNER_TEMP (on GitHub
    Actions) or TEMP.  An existing cluster is reused rather than recreated.

.PARAMETER LogFile
    Server log.  Defaults to a file next to the cluster directory.

.PARAMETER Database
    Database created for the tests.  Must match DB_NAME in embed_db_info_windows.sh.

.PARAMETER User
    Role that owns the cluster.  Must match DB_USER in embed_db_info_windows.sh.

.PARAMETER PgBin
    PostgreSQL bin directory.  Defaults to PGBIN, then to the newest numbered
    directory under C:\Program Files\PostgreSQL.

.PARAMETER Stop
    Stop a cluster this script started instead of starting one.  The cluster
    directory is left in place.

.EXAMPLE
    .\start-test-postgresql.ps1

.EXAMPLE
    .\start-test-postgresql.ps1 -Port 55433 -DataDir C:\pg\esql-test

.EXAMPLE
    .\start-test-postgresql.ps1 -Stop
#>
[CmdletBinding()]
param(
    [int]    $Port = 55432,
    [string] $DataDir,
    [string] $LogFile,
    [string] $Database = 'testdb',
    [string] $User = 'main_user',
    [string] $PgBin,
    [switch] $Stop
)

$ErrorActionPreference = 'Stop'
# PowerShell 7.4 turns a non-zero exit from a native command into a terminating
# error when $ErrorActionPreference is 'Stop'.  That would bypass the exit-code
# checks below, which are the ones that print the server log.  Assigning this
# is harmless on the older PowerShell versions that do not know the variable.
$PSNativeCommandUseErrorActionPreference = $false

function Resolve-PgBin {
    param([string] $Requested)

    foreach ($candidate in @($Requested, $env:PGBIN)) {
        if ($candidate -and (Test-Path (Join-Path $candidate 'pg_ctl.exe'))) {
            return $candidate
        }
    }

    $root = 'C:\Program Files\PostgreSQL'
    if (Test-Path $root) {
        $newest = Get-ChildItem $root -Directory |
            Where-Object { $_.Name -match '^\d+$' } |
            Sort-Object { [int]$_.Name } -Descending |
            Select-Object -First 1
        if ($newest) {
            $bin = Join-Path $newest.FullName 'bin'
            if (Test-Path (Join-Path $bin 'pg_ctl.exe')) {
                return $bin
            }
        }
    }

    throw "No PostgreSQL installation found. Pass -PgBin or set PGBIN to the directory holding pg_ctl.exe."
}

function Show-ServerLog {
    param([string] $Log)

    if ($Log -and (Test-Path $Log)) {
        Write-Host "--- $Log ---"
        Get-Content $Log
        Write-Host '--- end of server log ---'
    }
}

# Runs a PostgreSQL command line tool, showing the server log if it fails.
function Invoke-PgTool {
    param(
        [string]   $Exe,
        [string[]] $PgArguments,
        [string]   $Log
    )

    & (Join-Path $script:pgBinDir $Exe) @PgArguments
    if ($LASTEXITCODE -ne 0) {
        Show-ServerLog -Log $Log
        throw "$Exe failed with exit code $LASTEXITCODE"
    }
}

$script:pgBinDir = Resolve-PgBin -Requested $PgBin

if (-not $DataDir -or -not $LogFile) {
    $base = $env:RUNNER_TEMP
    if (-not $base) { $base = $env:TEMP }
    if (-not $DataDir) { $DataDir = Join-Path $base 'oc4j-esql-pgdata' }
    if (-not $LogFile) { $LogFile = Join-Path $base 'oc4j-esql-postgresql.log' }
}

$clusterExists = Test-Path (Join-Path $DataDir 'PG_VERSION')

$isRunning = $false
if ($clusterExists) {
    # pg_ctl status exits 0 when the server is up and 3 when it is not.  Its
    # stderr is deliberately not redirected: turning native stderr into error
    # records while $ErrorActionPreference is 'Stop' makes it terminating.
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    & (Join-Path $script:pgBinDir 'pg_ctl.exe') status -D $DataDir | Out-Null
    $isRunning = ($LASTEXITCODE -eq 0)
    $ErrorActionPreference = $previous
}

if ($Stop) {
    if ($isRunning) {
        Invoke-PgTool -Exe 'pg_ctl.exe' -PgArguments @('-D', $DataDir, '-w', 'stop') -Log $LogFile
        Write-Host "Stopped the cluster in $DataDir."
    } else {
        Write-Host "No running cluster in $DataDir; nothing to stop."
    }
    return
}

Write-Host "Using PostgreSQL from $script:pgBinDir"

if ($clusterExists) {
    Write-Host "Reusing the cluster already in $DataDir"
} else {
    Write-Host "Initialising a cluster in $DataDir"
    Invoke-PgTool -Exe 'initdb.exe' `
        -PgArguments @('-D', $DataDir, '-U', $User, '--auth=trust', '--encoding=UTF8', '--no-locale')
}

if ($isRunning) {
    Write-Host "The cluster is already running; not starting it again."
} else {
    Write-Host "Starting the server on port $Port (log: $LogFile)"
    Invoke-PgTool -Exe 'pg_ctl.exe' `
        -PgArguments @('-D', $DataDir, '-l', $LogFile, '-o', "-p $Port", '-w', 'start') -Log $LogFile
}

# createdb fails if the database is there, which it is whenever an existing
# cluster is reused, so ask first.
$existing = & (Join-Path $script:pgBinDir 'psql.exe') `
    -h localhost -p $Port -U $User -d postgres -tAc `
    "SELECT 1 FROM pg_database WHERE datname = '$Database'"
if ($LASTEXITCODE -ne 0) {
    Show-ServerLog -Log $LogFile
    throw "Could not reach the server on port $Port (psql exit code $LASTEXITCODE)"
}

if ($existing -eq '1') {
    Write-Host "Database $Database already exists."
} else {
    Write-Host "Creating database $Database"
    Invoke-PgTool -Exe 'createdb.exe' `
        -PgArguments @('-h', 'localhost', '-p', "$Port", '-U', $User, $Database) -Log $LogFile
}

Write-Host ''
Write-Host 'PostgreSQL is ready for the ESQL suites:'
Write-Host "  host     localhost"
Write-Host "  port     $Port"
Write-Host "  database $Database"
Write-Host "  user     $User"
Write-Host 'Copy .github/workflows/db-settings/embed_db_info_windows.sh over'
Write-Host 'tests/embed_db_info.sh if these differ from what is in it.'
