#/bin/sh
# Stop after the preprocessing stage; do not run the compiler proper.  The output is in the form of preprocessed source code, which is sent to the standard output.
g++ -I ./include/ -E ./source/example/EchoClient.cc -o ./echoclient.i
g++ -I ./include/ -E ./source/example/EchoServer.cc -o ./echoserver.i
