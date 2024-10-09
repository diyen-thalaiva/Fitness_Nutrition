#!/usr/bin/env pwsh

param (
    [string]$option
)

if (-not $option) {
    Write-Host "Usage: .\script.ps1 -option [build | run]"
    exit 1
}

switch ($option) {
    "build" {
        gcc src/main.c src/ai.c src/fitness.c src/nutrition.c src/user.c `
            -L "C:/Users/diyen/Downloads/curl-8.10.0_1-win64-mingw/curl-8.10.0_1-win64-mingw/lib" `
            -I "C:/Users/diyen/Downloads/curl-8.10.0_1-win64-mingw/curl-8.10.0_1-win64-mingw/include" `
            -lcurl -o build/app.exe
        Write-Host "Built app."
    }
    "run" {
        Write-Host "Running app."
        & ./build/app.exe
    }
    default {
        Write-Host "Invalid argument: $option"
        Write-Host "Usage: .\script.ps1 -option [build | run]"
        exit 1
    }
}
