#/bin/sh
./bin/Client -ip=localhost -port=8888 -log=client.log &
./bin/Client -ip=localhost -port=8888 -log=client1.log &
./bin/Client -ip=localhost -port=8888 -log=client2.log &