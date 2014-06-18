@echo off

if "%1"=="" goto default
if "%1"=="all" goto default
if "%1"=="clean" goto clean
if "%1"=="distclean" goto distclean
if "%1"=="test" goto test
if "%1"=="testclean" goto testclean
if "%1"=="release" goto release
if "%1"=="binder" goto binder
if "%1"=="--help" goto hlp
if "%1"=="-h" goto hlp

goto module

:hlp
echo "m is the make tool, based on CMake system."
echo "To use m, enter the main directory and type 'm' with options or targets."
echo ""

echo "Usage: m [options] [target] ..."
echo "Options:"
echo "  -h,--help          ""output usage information and exit"
echo "Targets"
echo "                     ""default build all c++ module"
echo "  all                ""build all c++ module"
echo "  clean              ""clean module"
echo "  test               ""build all unit-tests"
echo "  testclean          ""clean all unit-tests"
echo "  release            ""rebuild then package the output to release"
echo "  distclean          ""clean all target binary files and intermediate files"
echo "  binder             ""build nodejs plugin ControlBinder"
echo "  common             ""build Common"
echo "  dryetch            ""build DryEtch"
echo ""
echo "Examples:"
echo "  m clean"
echo "  m dryetch"
echo "  m release dryetch"
goto quit


:default
if not exist Build mkdir Build
cd Build
cmake -G"MinGW Makefiles" ..
make
cd ..
goto quit

:clean
if not exist Build goto quit
cd Build
make clean & cd ..
goto quit

:distclean
if exist Build rmdir /S /Q Build
if exist Output rmdir /S /Q Output
del /Q Release\*.*
goto quit

:release
if "%2"=="" echo "module name is empty." && goto quit
if not exist Build mkdir Build
cd Build
cmake -G"MinGW Makefiles" ..
make install
cd ..
7z a ./Release/%2.7z ./Output/%2/
goto quit

:test
if not exist Build mkdir Build
cd Build
cmake -G"MinGW Makefiles" ..
make test
cd ..
goto quit

:testclean
goto quit

:binder
set mod="debug"
set flag="0"
if "%2"=="-d" set flag="1"
if "%2"=="-r" set flag="1" && set mod="release"
if "%2"=="" set flag="1"
if %flag%=="0" echo "wrong compile parameter." & goto quit
xcopy /I/Y Code\Binder Build\Binder
cd Build/Binder
nw-gyp configure --%mod% --target=0.8.5 && nw-gyp build --%mod% --target=0.8.5 & cd ../.. & xcopy /Y Build\Binder\build\%mod%\*.node Bin
goto quit

:module
if not exist Code/%1 echo %1 not exist. && goto quit
if not exist Build mkdir Build
cd Build
cmake -G"MinGW Makefiles" ..
cd Code/%1
make
cd ../../..
goto quit

:quit