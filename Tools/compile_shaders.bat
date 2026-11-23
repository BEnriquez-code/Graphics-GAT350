@echo off
setlocal

set SHADERCROSS=%~dp0Shadercross\shadercross.exe
set INPUT=%~1
set OUTPUT=%~2

if "%INPUT%"=="" goto usage
if "%OUTPUT%"=="" goto usage
if not exist "%OUTPUT%" mkdir "%OUTPUT%"

echo Compiling shaders...
call :compile vert vertex
call :compile frag fragment

if defined FAILED (
    echo SHADER COMPILATION FAILED
    exit /b 1
)
echo Shader compilation complete.
exit /b 0

:compile
for %%f in ("%INPUT%*.%1.hlsl") do (
    echo Compiling %%~nxf
    "%SHADERCROSS%" "%%f" -s HLSL -d SPIRV -t %2 -e main -I "%INPUT%." -o "%OUTPUT%%%~nf.spv"  || set FAILED=1
    "%SHADERCROSS%" "%%f" -s HLSL -d DXIL  -t %2 -e main -I "%INPUT%." -o "%OUTPUT%%%~nf.dxil" || set FAILED=1
    "%SHADERCROSS%" "%%f" -s HLSL -d JSON  -t %2 -e main -I "%INPUT%." -o "%OUTPUT%%%~nf.json" || set FAILED=1
)
exit /b

:usage
echo Usage: compile_shaders.bat shader_dir\ output_dir\
exit /b 1