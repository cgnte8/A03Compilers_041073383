@echo off
REM
REM Run this script to build and test the biology program
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

gcc -std=gnu99 -fcommon -w -o biology.exe ^
src\Biology.c ^
src\Main1Cell.c ^
src\Step1Cell.c ^
src\Step2Tissue.c ^
src\Main2Tissue.c ^
-lm

if errorlevel 1 (
echo [ERROR] Build failed.
exit /b 1
)

echo.
echo === [1] Cell: Encode STEL_CODE1.txt -^> CODED.TXT ===
biology.exe 1 1 txtFiles\STEL_CODE1.txt txtFiles\CODED.txt

if errorlevel 1 (
echo [ERROR] Cell encoding failed.
exit /b 1
)

echo.
echo === [2] Tissue: Decode CODED.txt -^> A02_DECODED.txt ===
biology.exe 2 txtFiles\CODED.txt txtFiles\A02_DECODED.txt 2>txtFiles\A02_ERRORS.err

if errorlevel 1 (
echo [ERROR] Tissue decoding failed.
exit /b 1
)

echo.
echo === Verifying Tissue output ===

REM ------------------------------------------------------------
REM Verify that the Tissue decoded file matches the original
REM ------------------------------------------------------------
fc /b txtFiles\STEL_CODE1.txt txtFiles\A02_DECODED.txt >nul

if errorlevel 1 (
echo [FAIL] A02_DECODED.txt does not match STEL_CODE1.txt.
exit /b 1
) else (
echo [ OK ] Tissue decoding verified.
)

echo.
echo === All tests passed! ===

exit /b 0
