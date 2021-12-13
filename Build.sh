#/bin/sh
make clean
make server BUILDFLAGS=-DBUILD_SERVER
make clean
make client BUILDFLAGS=-DBUILD_CLIENT
make clean
