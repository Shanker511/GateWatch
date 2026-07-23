$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$OverlayDir = Split-Path -Parent $ScriptDir
$WorkDir = if ($env:GATEWATCH_WORK_DIR) { $env:GATEWATCH_WORK_DIR } else { Join-Path $OverlayDir ".work" }
$MeshtasticDir = Join-Path $WorkDir "meshtastic"
$MeshtasticRef = if ($env:MESHTASTIC_REF) { $env:MESHTASTIC_REF } else { "master" }

New-Item -ItemType Directory -Force -Path $WorkDir | Out-Null
if (-not (Test-Path (Join-Path $MeshtasticDir ".git"))) {
    git clone https://github.com/meshtastic/firmware.git $MeshtasticDir
}

git -C $MeshtasticDir fetch --tags origin
git -C $MeshtasticDir checkout --force $MeshtasticRef
git -C $MeshtasticDir submodule update --init --recursive
python (Join-Path $ScriptDir "apply_overlay.py") $MeshtasticDir

try { pio --version | Out-Null }
catch {
    python -m pip install --user platformio
}

Push-Location $MeshtasticDir
try {
    python -m platformio run -e gatewatch-xiao-s3
}
finally {
    Pop-Location
}

Write-Host "Build output: $MeshtasticDir\.pio\build\gatewatch-xiao-s3"
