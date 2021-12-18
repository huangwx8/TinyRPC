#/bin/sh
nohup ./bin/EchoServer -log=server.log &
./bin/EchoClient -ip=localhost -port=8888 -log=client1.log &
./bin/EchoClient -ip=localhost -port=8888 -log=client2.log &
./bin/EchoClient -ip=localhost -port=8888 -log=client3.log &
