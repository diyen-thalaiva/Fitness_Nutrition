@echo off
setlocal

:: Set error handling to stop on error
if "%1"=="" (
    echo Usage: script.bat option[build ^| run]
    exit /b 1
)

:: Switch based on the argument passed
if "%1"=="build" (
    gcc src\ai.c src\fitness.c src\menu.c src\nutrition.c src\users.c src\utils.c src\main.c ^
        -LC:\Users\lomes\OneDrive\Desktop\curl-8.10.0_1-win64-mingw\lib -lcurl ^
        -IC:\Users\lomes\OneDrive\Desktop\curl-8.10.0_1-win64-mingw\include ^
        -LC:\Users\lomes\OneDrive\Desktop\PDCurses-3.8\wincon -lpdcurses ^
        -IC:\Users\lomes\OneDrive\Desktop\PDCurses-3.8 -o build\app.exe
    if %errorlevel% neq 0 (
        echo Error: Build failed.
        exit /b 1
    )
    echo Built app.
) else if "%1"=="run" (
    echo Running app.
    build\app.exe
) else (
    echo Invalid argument: %1
    echo Usage: script.bat option[build ^| run]
    exit /b 1
)

endlocal
