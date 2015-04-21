#!/bin/bash

echo "Setting up Apache Thrift."

# Required tools and libs for Thrift.
sudo apt-get install libboost-dev libboost-test-dev libboost-program-options-dev libboost-system-dev libboost-filesystem-dev libevent-dev automake libtool flex bison pkg-config g++ libssl-dev

# Install javac.
sudo apt-get install openjdk-7-jdk

# Need a more recent version of automake.
wget http://ftp.debian.org/debian/pool/main/a/automake-1.14/automake_1.14.1-3_all.deb
sudo dpkg -i automake_1.14.1-3_all.deb

# C/C++ language support.
sudo apt-get install libglib2.0-dev

# Get Thrift repo.
git clone https://git-wip-us.apache.org/repos/asf/thrift.git thrift
cd thrift

# Build and install Thrift.
./bootstrap.sh
./configure
make
sudo su
make install
cd ..
sudo rm -rf thrift

echo "Hopefully done."
