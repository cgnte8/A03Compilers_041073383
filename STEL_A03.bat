@echo off
REM
REM Run this script to build and test the Stella program
REM for encoding and decoding text files.
REM
REM This script runs on Windows using GCC.
REM

REM ------------------------------------------------------------
REM Add MSYS2 UCRT64 GCC to PATH
REM ------------------------------------------------------------
set "PATH=C:\Connor\School\Programs\MSYS2\ucrt64\bin;%PATH%"

REM ------------------------------------------------------------
REM Run from the directory containing this script
REM ------------------------------------------------------------
cd /d "%~dp0"

echo Building Stella Compiler ...

gcc -std=gnu99 -fcommon -o STEL_A03.exe ^
src\Biology.c ^
src\Main1Cell.c ^
src\Step1Cell.c ^
src\Step2Tissue.c ^
src\Main2Tissue.c ^
src\Step3Organ.c ^
src\Main3Organ.c ^
-lm

if errorlevel 1 (
echo [ERROR] Build failed.
exit /b 1
)

REM Change this to whichever example you want to work with
set "INPUT_FILE=txtFiles\Input_Example5.txt"
REM ------------------------------------------------------

echo.
echo === [1] Cell: Encode Input_Example1-6 || Error_Example1.txt -^> CODED.TXT ===
STEL_A03.exe 1 1 %INPUT_FILE% txtFiles\CODED.txt

if errorlevel 1 (
echo [ERROR] Cell encoding failed.
exit /b 1
)

echo.
echo === [3] Organ: Decode and scan CODED.txt -^> A03_SCANNER.txt ===
STEL_A03.exe 3 txtFiles\CODED.txt txtFiles\compiler_scanner_dfa.txt >txtFiles\A03_SCANNER.txt 2>txtFiles\A03_ERRORS.err

if errorlevel 1 (
echo [ERROR] Organ scanning failed.
exit /b 1
)

echo.
echo === All tests passed! ===

exit /b 0
