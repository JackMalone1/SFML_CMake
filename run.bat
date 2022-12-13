md build
cd build
cmake -G "Visual Studio 17 2022 Win64" ..\ -Dgtest_force_shared_crt=on
cd x64-Debug
start "" GGJ2020.exe
PAUSE