$ErrorActionPreference = 'Stop'
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$ts = Join-Path $here 'Restaurant.am.ts'
$qm = Join-Path $here 'Restaurant.am.qm'

$candidates = @(
    (Join-Path $env:APPDATA 'Python\Python314\Scripts\pyside6-lrelease.exe'),
    (Join-Path $env:LOCALAPPDATA 'Programs\Python\Python314\Scripts\pyside6-lrelease.exe')
)

if ($env:QTDIR) {
    $candidates = @(Join-Path $env:QTDIR 'bin\lrelease.exe') + $candidates
}

$lrelease = $null
foreach ($c in $candidates) {
    if ($c -and (Test-Path -LiteralPath $c)) {
        $lrelease = $c
        break
    }
}

if (-not $lrelease) {
    $cmd = Get-Command lrelease -ErrorAction SilentlyContinue
    if ($cmd) { $lrelease = $cmd.Source }
}

if (-not $lrelease) {
    Write-Error 'lrelease not found (install Qt Linguist tools or: pip install pyside6)'
}

& $lrelease $ts -qm $qm
Write-Host "Updated $qm"
