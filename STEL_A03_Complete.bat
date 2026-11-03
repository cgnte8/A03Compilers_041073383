@echo off
REM
REM Run this script to build and Stress Test the Stella Program
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

gcc -std=gnu99 -fcommon -o STEL_Tests.exe ^
src\Biology.c ^
src\Main1Cell.c ^
src\Step1Cell.c ^
src\Test1Cell.c ^
src\Step2Tissue.c ^
src\Main2Tissue.c ^
src\Test2Tissue.c ^
src\Step3Organ.c ^
src\Main3Organ.c ^
src\Test3Organ.c ^
-lm

if errorlevel 1 (
echo [ERROR] Build failed.
exit /b 1
)

echo.
echo === [6] Test: Run all tests for Cell ===
STEL_Tests.exe 6 > txtFiles/testResults.txt

if errorlevel 1 (
	echo [ERROR] Cell tests failed.
	exit /b 1
)

echo.
echo === [7] Test: Run all tests for Tissue ===
STEL_Tests.exe 7 >> txtFiles/testResults.txt

if errorlevel 1 (
	echo [ERROR] Tissue tests failed.
	exit /b 1 
)

echo.
echo === [8] Test: Run all tests for Organ ===
STEL_Tests.exe 8 >> txtFiles\testResults.txt

if errorlevel 1 (
	echo [ERROR] Organ tests failed.
	exit /b 1
)

echo.
echo === All tests passed! ===

echo.
echo === Testing Input Examples ===

REM (Start, Step, End)
for /L %%i in (1,1,6) do (
		
	echo === Test %%i, Input_Example%%i ===

	echo.
	echo === [1] Cell: Encode Input_Example%%i -^> CODED.TXT ===
	STEL_Tests.exe 1 1 txtFiles\Input_Example%%i.txt txtFiles\CODED.txt

	if errorlevel 1 (
		echo [ERROR] Cell encoding failed.
		exit /b 1
	)

	echo.
	echo === [3] Organ: Decode and scan CODED.txt -^> testResults.txt ===
	STEL_Tests.exe 3 txtFiles\CODED.txt txtFiles\compiler_scanner_dfa.txt >> txtFiles\testResults.txt

	if errorlevel 1 (
		echo [ERROR] Organ scanning failed.
		exit /b 1
	)
)

echo.
echo === All Input Examples Passed! ===

echo.
echo === Testing Error Example ===

echo.
echo === [1] Cell: Encode Error_Example1.txt -^> CODED.TXT ===
STEL_Tests.exe 1 1 txtFiles\Error_Example1.txt txtFiles\CODED.txt

if errorlevel 1 (
	echo [ERROR] Cell encoding failed.
	exit /b 1
)

echo.
echo === [3] Organ: Decode and scan CODED.txt -^> testResults.txt ===
STEL_Tests.exe 3 txtFiles\CODED.txt txtFiles\compiler_scanner_dfa.txt >> txtFiles\testResults.txt

if errorlevel 1 (
	echo.
	echo [OK] Expected Error Detected. >> txtFiles\testResults.txt
	exit /b 0
)