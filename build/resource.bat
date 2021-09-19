@echo off
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\rc.exe" ../res/resource.rc
"C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.29.30037\bin\Hostx86\x86\link.exe" /NOENTRY /MACHINE:X86 /DLL ../res/resource.res
pause