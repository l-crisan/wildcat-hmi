set Config=%1

if "%Config%" == "LinuxARM64Debug" (
copy .\lib\%Config%\*.so .\build\%Config%
)

if "%Config%" == "LinuxARM64Release" (
copy .\lib\%Config%\*.so .\build\%Config%
)

if "%Config%" == "WinX86Release" (
copy .\lib\%Config%\*.dll .\build\%Config%
)

if "%Config%" == "WinX86Debug" (
copy .\lib\%Config%\*.dll .\build\%Config%
)

if "%Config%" == "WinX64Release" (
copy .\lib\%Config%\*.dll .\build\%Config%
)

if "%Config%" == "WinX64Debug" (
copy .\lib\%Config%\*.dll .\build\%Config%
)
