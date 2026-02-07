taskkill /F /IM nmake.exe
taskkill /F /IM qmake.exe
rmdir /s /q thirdparty\qwt-lib
git clone https://github.com/opencor/qwt.git thirdparty/qwt-lib
