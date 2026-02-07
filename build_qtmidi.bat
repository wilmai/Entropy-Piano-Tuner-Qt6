call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
set PATH=%PATH%;C:\Qt\6.10.2\msvc2022_64\bin
cd qtmidi
if exist .qmake.stash del .qmake.stash
qmake -spec win32-msvc
nmake
nmake install
