set SolutionDir=%~dp0
set Config=%1
set ProjectName=%2
set Top=%SolutionDir%

cd %Top%

if "%Config%" == "LinuxARMDebug" (
call jam.bat configure --debug -sCONFIG=%Config% -sTOOLSET=gcc -sTARGET_OSPLAT=arm -sTOOLSET_ROOT=%ENSO_LINUX_ARM32_GCC% -sTOOLSET_PREFIX="arm-linux-gnueabihf-"  -sTARGET_OS=linux -sGUI=linux-fb
)

if "%Config%" == "LinuxARMRelease" (
call jam.bat configure --optimize -sCONFIG=%Config% -sTOOLSET=gcc -sTARGET_OSPLAT=arm -sTOOLSET_ROOT=%ENSO_LINUX_ARM32_GCC% -sTOOLSET_PREFIX="arm-linux-gnueabihf-"  -sTARGET_OS=linux -sGUI=linux-fb
)

if "%Config%" == "LinuxARM64Debug" (
call jam.bat configure --debug -sCONFIG=%Config% -sTOOLSET=gcc -sTARGET_OSPLAT=arm -sTOOLSET_ROOT=%WILDCAT_LINUX_ARM64_GCC%  -sTOOLSET_PREFIX="aarch64-none-linux-gnu-"  -sTARGET_OS=linux -sGUI=linux-fb
)

if "%Config%" == "LinuxARM64Release" (
call jam.bat configure --optimize -sCONFIG=%Config% -sTOOLSET=gcc -sTARGET_OSPLAT=arm -sTOOLSET_ROOT=%WILDCAT_LINUX_ARM64_GCC%  -sTOOLSET_PREFIX="aarch64-none-linux-gnu-" -sTARGET_OS=linux -sGUI=linux-fb
)

if "%Config%" == "WinX86Debug" (
call jam.bat configure --debug -sCONFIG=%Config%  -sTARGET_OSPLAT=x86 -sGUI=win32
)

if "%Config%" == "WinX86Release" (
call jam.bat configure --optimize -sCONFIG=%Config%  -sTARGET_OSPLAT=x86 -sGUI=win32
)

if "%Config%" == "WinX64Debug" (
call jam.bat configure --debug -sCONFIG=%Config%  -sTARGET_OSPLAT=x86_64 -sGUI=win32
)

if "%Config%" == "WinX64Release" (
call jam.bat configure --optimize -sCONFIG=%Config%  -sTARGET_OSPLAT=x86_64 -sGUI=win32
)

if "%Config%" == "WebRelease" (
call jam.bat  configure  --optimize -sCONFIG=%Config%  -sTOOLSET=emcc -sTARGET_OSPLAT=wasm32 -sEMSDK=%EMCC_SDK% -sTARGET_OS=emscripten -sGUI=emscripten
)

if "%Config%" == "WebDebug" (
call jam.bat  configure --debug -sCONFIG=%Config%  -sTOOLSET=emcc -sTARGET_OSPLAT=wasm32 -sEMSDK=%EMCC_SDK% -sTARGET_OS=emscripten -sGUI=emscripten
)

call jam.bat -sTOP=%Top% -q -j8  %ProjectName% 
