#/bin/sh
./bin/EchoClient -ip=localhost -port=8888 -log=client.log &
./bin/EchoClient -ip=localhost -port=8888 -log=client1.log &
./bin/EchoClient -ip=localhost -port=8888 -log=client2.log &