@echo off
REM Removes the executable and every generated file
echo Cleaning...
if exist *.exe del /q *.exe 2>nul
if exist *.o del /q *.o 2>nul
if exist txtFiles\CODED.txt del /q txtFiles\CODED.txt 2>nul
if exist txtFiles\DECODED.txt del /q txtFiles\DECODED.txt 2>nul
if exist txtFiles\testResults.txt del /q txtFiles\testResults.txt 2>nul
if exist txtFiles\A03_Errors.err del /q txtFiles\A03_ERRORS.err 2>nul
if exist txtFiles\A03_SCANNER.txt del /q txtFiles\A03_SCANNER.txt 2>nul
if exist txtFiles\output.txt del /q txtFiles\output.txt 2>nul
if exist build\ rmdir /s /q build 2>nul
echo Everything Successfully Deleted
echo.
