param (
  [string]$command = "build",
  [switch]$debug = $false
)

function DebugLog {
  param (
    [string]$format,
    [string]$type = "info"
  )
  if ($debug -eq $false) {
    return
  }
  $foregroundColor = $null
  $backgroundColor = $null
  switch ($type) {
    "warning" {
      $foregroundColor = [ConsoleColor]::Blue
      $backgroundColor = [ConsoleColor]::Yellow
    }
  }
  if ($null -ne $foregroundColor) {
    Write-Host $format -ForegroundColor $foregroundColor -BackgroundColor $backgroundColor
  }
  else {
    Write-Host $format
  }
}

function SetEnv() {
  param(
    [string]$path = ".\.env"
  )
  ForEach ($line in (Get-Content -Path $path)) {
    $key, $value = $line.Split("=")
    DebugLog ([string]::Format("{0} = {1}", $key, $value)) -type "warning"
    Set-Item -Path Env:${key} -Value $value
  }
}

Write-Host "vcbuild.ps1 starting..."
SetEnv
switch ($command) {
  "build" { 
    # todo
  }
  Default {}
}

Write-Host "vcbuild.ps1 exit"
