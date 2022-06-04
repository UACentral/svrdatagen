# svrdatagen
UA Server Data Generator

## Pre-Req
Install tools
```
sudo apt install build-essential
sudo apt install git
sudo apt install cmake
```
Clone OPC UA Open62541
```
mkdir ~/prj
cd ~/prj
git clone https://github.com/open62541/open62541.git
```
Build
```
git submodule update --init --recursive
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUA_NAMESPACE_ZERO=FULL -DUA_ENABLE_AMALGAMATION=ON ..
make
sudo make install
```
