call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
mkdir Build
cd Build
cmake -DCMAKE_BUILD_TYPE=Debug -DCOMPILE_UNIT_TESTS=On ../
pause