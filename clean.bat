IF NOT EXIST ./build mkdir build

cd ./build

mingw32-make.exe clean

cd ..

cls

rmdir /s /q build