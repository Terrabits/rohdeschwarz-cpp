@echo off
setlocal
SET "ROOT_DIR=%~dp0..\.."
cd /d "%ROOT_DIR%"


REM install
call scripts\install.bat --settings build_type=Debug %*