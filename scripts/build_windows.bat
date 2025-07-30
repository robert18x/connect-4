@echo off

pushd %~dp0
cd ..

IF NOT EXIST .venv python -m venv .venv
call .venv\Scripts\activate.bat
python -m pip install conan

IF EXIST build rmdir /s /q build || exit /b %errorlevel%
mkdir build
cd build

conan install .. ^
    -s:a build_type=Debug ^
    -s:a compiler.runtime=dynamic ^
    -s:a compiler.version=194 ^
    --build missing ^
    -o "&:build_gui=True" ^
    -o "&:build_tests=True" || exit /b %errorlevel%

cmake --preset conan-default .. || exit /b %errorlevel%

popd
