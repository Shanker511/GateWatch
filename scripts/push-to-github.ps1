$ErrorActionPreference = "Stop"

$RepoUrl = "https://github.com/Shanker511/GateWatch.git"
$PackageRoot = Split-Path -Parent $PSScriptRoot
$Stamp = Get-Date -Format "yyyyMMdd-HHmmss"
$WorkDir = Join-Path ([System.IO.Path]::GetTempPath()) "GateWatch-push-$Stamp"

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "Git is not installed or is not in PATH."
}

Write-Host "Cloning the existing GitHub repository..."
git clone $RepoUrl $WorkDir
if ($LASTEXITCODE -ne 0) { throw "Git clone failed." }

Write-Host "Copying the GateWatch package into the cloned repository..."
Get-ChildItem -Path $PackageRoot -Force |
    Where-Object { $_.Name -ne ".git" } |
    Copy-Item -Destination $WorkDir -Recurse -Force

Set-Location $WorkDir
python -m unittest discover -s simulator -v
if ($LASTEXITCODE -ne 0) { throw "Simulator tests failed." }

git add --all
$changes = git status --porcelain
if (-not $changes) {
    Write-Host "No changes to commit."
    Write-Host "Working copy: $WorkDir"
    exit 0
}

git commit -m "Add GateWatch fence monitoring prototype"
if ($LASTEXITCODE -ne 0) {
    throw "Git could not create the commit. Configure your Git name/email if prompted."
}

git push origin HEAD:main
if ($LASTEXITCODE -ne 0) { throw "Git push failed." }

Write-Host "GateWatch was pushed successfully."
Write-Host "Local working copy: $WorkDir"
