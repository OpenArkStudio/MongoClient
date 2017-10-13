@echo off

echo Building dependencies...

REM ######################################################################################################
echo Building boost...

if exist boost (rd boost /q /s)

REM git config --global http.sslVerify false
set GIT_SSL_NO_VERIFY=true
git clone --recursive -b boost-1.65.0 https://github.com/boostorg/boost.git

cd boost
call .\bootstrap.bat

.\b2 --build-type=complete toolset=msvc-14.0 address-model=64 threading=multi debug release stage

REM git拉下来的boost 生成的boost文件夹缺少头文件
mklink boost\utility\string_ref.hpp ..\..\libs\utility\include\boost\utility\string_ref.hpp

cd ..\
REM ######################################################################################################
echo Building mongo-c-driver...

if exist mongo-c-driver (rd mongo-c-driver /q /s)
git clone --recursive -b 1.8.1 https://github.com/mongodb/mongo-c-driver.git

cd mongo-c-driver
cd src\libbson\build

cmake -G "Visual Studio 14 2015 Win64" "-DCMAKE_INSTALL_PREFIX=..\..\..\..\mongo-driver\Debug" "-DCMAKE_BUILD_TYPE=Debug" ..
"%VS140COMNTOOLS%..\IDE\Devenv" libbson.sln /build "Debug|x64" /project ALL_BUILD.vcxproj
"%VS140COMNTOOLS%..\IDE\Devenv" libbson.sln /build "Debug|x64" /project INSTALL.vcxproj

cmake -G "Visual Studio 14 2015 Win64" "-DCMAKE_INSTALL_PREFIX=..\..\..\..\mongo-driver\Release" "-DCMAKE_BUILD_TYPE=Release" ..
"%VS140COMNTOOLS%..\IDE\Devenv" libbson.sln /build "Release|x64" /project ALL_BUILD.vcxproj
"%VS140COMNTOOLS%..\IDE\Devenv" libbson.sln /build "Release|x64" /project INSTALL.vcxproj

cd ..\..\..\build
cmake -G "Visual Studio 14 2015 Win64" "-DCMAKE_INSTALL_PREFIX=..\..\mongo-driver\Debug" "-DCMAKE_BUILD_TYPE=Debug" "-DCMAKE_PREFIX_PATH=..\..\mongo-driver\Debug" "-DENABLE_SSL=WINDOWS" "-DENABLE_SASL=SSPI" "-DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF" ..
"%VS140COMNTOOLS%..\IDE\Devenv" libmongoc.sln /build "Debug|x64" /project ALL_BUILD.vcxproj
"%VS140COMNTOOLS%..\IDE\Devenv" libmongoc.sln /build "Debug|x64" /project INSTALL.vcxproj

cmake -G "Visual Studio 14 2015 Win64" "-DCMAKE_INSTALL_PREFIX=..\..\mongo-driver\Release" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_PREFIX_PATH=..\..\mongo-driver\Release" "-DENABLE_SSL=WINDOWS" "-DENABLE_SASL=SSPI" "-DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF" ..
"%VS140COMNTOOLS%..\IDE\Devenv" libmongoc.sln /build "Release|x64" /project ALL_BUILD.vcxproj
"%VS140COMNTOOLS%..\IDE\Devenv" libmongoc.sln /build "Release|x64" /project INSTALL.vcxproj

cd ..\..\
REM ######################################################################################################
echo Building mongo-cxx-driver...

if exist mongo-cxx-driver (rd mongo-cxx-driver /q /s)
git clone --recursive -b r3.1.3 https://github.com/mongodb/mongo-cxx-driver.git

cd mongo-cxx-driver
cd build

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=..\..\mongo-driver\Debug -DLIBMONGOC_DIR=..\..\..\mongo-driver\Debug -DLIBBSON_DIR=..\..\..\mongo-driver\Debug -DBSONCXX_POLY_USE_BOOST=1 -DBOOST_ROOT=..\..\..\boost -DMONGOCXX_ENABLE_SSL=ON -DBUILD_SHARED_LIBS=ON ..
"%VS140COMNTOOLS%..\IDE\Devenv" MONGO_CXX_DRIVER.sln /build "Debug|x64" /project ALL_BUILD.vcxproj
"%VS140COMNTOOLS%..\IDE\Devenv" MONGO_CXX_DRIVER.sln /build "Debug|x64" /project INSTALL.vcxproj

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=..\..\mongo-driver\Release -DLIBMONGOC_DIR=..\..\..\mongo-driver\Release -DLIBBSON_DIR=..\..\..\mongo-driver\Release -DBSONCXX_POLY_USE_BOOST=1 -DBOOST_ROOT=..\..\..\boost -DMONGOCXX_ENABLE_SSL=ON -DBUILD_SHARED_LIBS=ON ..
"%VS140COMNTOOLS%..\IDE\Devenv" MONGO_CXX_DRIVER.sln /build "Release|x64" /project ALL_BUILD.vcxproj
"%VS140COMNTOOLS%..\IDE\Devenv" MONGO_CXX_DRIVER.sln /build "Release|x64" /project INSTALL.vcxproj

cd ..\..\
REM ######################################################################################################
