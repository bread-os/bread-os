param (
    [switch]$build = $false,
    [switch]$start = $false,
    [switch]$debug = $false
)

function DebugLog
{
    param (
        [string]$format,
        [string]$type = "info"
    )
    if ($debug -eq $false)
    {
        return
    }
    $foregroundColor = $null
    $backgroundColor = $null
    switch ($type)
    {
        "warning" {
            $foregroundColor = [ConsoleColor]::Blue
            $backgroundColor = [ConsoleColor]::Yellow
        }
    }
    if ($null -ne $foregroundColor)
    {
        Write-Host $format -ForegroundColor $foregroundColor -BackgroundColor $backgroundColor
    }
    else
    {
        Write-Host $format
    }
}

function SetEnv()
{
    param(
        [string]$path = ".\.env"
    )
    ForEach ($line in (Get-Content -Path $path))
    {
        $key, $value = $line.Split("=")
        DebugLog ([string]::Format("{0} = {1}", $key, $value)) -type "warning"
        Set-Item -Path Env:${key} -Value $value
    }
}

DebugLog "vcbuild.ps1 starting..."
SetEnv
if ($build)
{
    Write-Host "start building..."
    if ($null -ne $args)
    {
        wsl.exe make $args
    }
    else
    {
        wsl.exe make all
    }
    Write-Host "build success!"
}
elseif ($start)
{
    Write-Host "start qemu"
    qemu-system-x86_64 `
        -cpu qemu64 -cpu qemu64 `
        -bios $Env:OVMF_PATH `
        -drive file=$Env:OUTPUT_DIR/uefi.img,format=raw,if=ide `
        -net none
}
else
{
    Write-Host "Usage: winbuild.ps1 [options] [arguments]"
    Write-Host "Example:"
    Write-Host "    winbuild.ps1 -build -debug"
    Write-Host "Options:"
    Write-Host "    -build `t` build porject"
    Write-Host "    -debug `t` enable debug logger"
}

DebugLog "vcbuild.ps1 exit"
