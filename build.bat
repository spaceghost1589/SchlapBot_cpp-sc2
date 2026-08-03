@echo off
echo Configuring SchlapBot for 64-bit...
cmake --preset x64-RelWithDebInfo

echo Building project...
cmake --build out/build/x64-RelWithDebInfo --parallel

echo Done! You can now open this folder in Visual Studio safely.
pause