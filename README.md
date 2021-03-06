# svrdatagen
UA Server Data Generator

## Pre-Req
Install tools
```
sudo apt install build-essential
sudo apt install git
sudo apt install cmake
```
Clone OPC UA Open62541 and build
```
mkdir ~/prj
cd ~/prj
git clone https://github.com/open62541/open62541.git
cd open62541
git checkout v1.3.2
git submodule update --init --recursive
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUA_NAMESPACE_ZERO=FULL -DUA_ENABLE_AMALGAMATION=ON ..
make
sudo make install
```
## Initial setup
Clone git repo
```
cd ~/prj
git clone https://github.com/UACentral/svrdatagen.git
```
Soft link open62541 .c/.h
```
cd ~/prj/svrdatagen
ln -s ~/prj/open62541/build/open62541.c ./open62541.c
ln -s ~/prj/open62541/build/open62541.h ./open62541.h
```
Build
```
gcc -std=c99 open62541.c svrdatagen.c -o svrdatagen
```
Build with debugging info
```
gcc -std=c99 -ggdb open62541.c svrdatagen.c -o svrdatagen
```