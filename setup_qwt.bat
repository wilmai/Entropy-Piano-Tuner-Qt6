taskkill /F /IM nmake.exe
taskkill /F /IM qmake.exe
timeout /t 2 /nobreak
rmdir /s /q thirdparty\qwt-lib
rmdir /s /q thirdparty\qwt-lib
git clone --branch qwt-6.2 https://github.com/UweR/qwt.git thirdparty/qwt-lib
