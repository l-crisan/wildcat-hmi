call .\vs-jam-clean.bat LinuxARM64Debug
call .\vs-jam-build.bat LinuxARM64Debug -a

call .\vs-jam-clean.bat LinuxARM64Release
call .\vs-jam-build.bat LinuxARM64Release -a

call .\vs-jam-clean.bat LinuxARMDebug
call .\vs-jam-build.bat LinuxARMDebug -a

call .\vs-jam-clean.bat LinuxARMRelease
call .\vs-jam-build.bat LinuxARMRelease -a

call .\vs-jam-clean.bat WinX64Debug
call .\vs-jam-build.bat WinX64Debug -a

call .\vs-jam-clean.bat WinX64Release
call .\vs-jam-build.bat WinX64Release -a

call .\vs-jam-clean.bat WinX86Debug
call .\vs-jam-build.bat WinX86Debug -a

call .\vs-jam-clean.bat WinX86Release
call .\vs-jam-build.bat WinX86Release -a

pause