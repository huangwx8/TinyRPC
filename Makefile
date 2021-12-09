CC=g++
CFLAGS=-g -std=c++11
SERVERFLAGS=-DBUILD_SERVER
CLIENTFLAGS=-DBUILD_CLIENT

INCLUDE=-I${PWD}/include
SRCDIR=${PWD}/source
OBJDIR=${PWD}/obj
OUTDIR=${PWD}/binary

COMMONOBJECTS=$(patsubst ${SRCDIR}/common/%.cc,${OBJDIR}/common/%.o,$(wildcard ${SRCDIR}/common/*.cc))
SERIALIZATIONOBJECTS=$(patsubst ${SRCDIR}/serialization/%.cc,${OBJDIR}/serialization/%.o,$(wildcard ${SRCDIR}/serialization/*.cc))
RUNTIMEOBJECTS=$(patsubst ${SRCDIR}/runtime/%.cc,${OBJDIR}/runtime/%.o,$(shell find ${SRCDIR}/runtime/ -name '*.cc'))
TRANSPORTOBJECTS=$(patsubst ${SRCDIR}/transport/%.cc,${OBJDIR}/transport/%.o,$(wildcard ${SRCDIR}/transport/*.cc))
SERVEROBJECTS=$(patsubst ${SRCDIR}/server/%.cc,${OBJDIR}/server/%.o,$(wildcard ${SRCDIR}/server/*.cc))
CLIENTOBJECTS=$(patsubst ${SRCDIR}/client/%.cc,${OBJDIR}/client/%.o,$(wildcard ${SRCDIR}/client/*.cc))

OUTSERVER=${OUTDIR}/EchoServer
OUTCLIENT=${OUTDIR}/EchoClient
OUTSERVEROBJECTS=${OBJDIR}/example/EchoServer.o ${COMMONOBJECTS} ${SERIALIZATIONOBJECTS} ${RUNTIMEOBJECTS} ${TRANSPORTOBJECTS} ${CLIENTOBJECTS} ${SERVEROBJECTS}
OUTCLIENTOBJECTS=${OBJDIR}/example/EchoClient.o ${COMMONOBJECTS} ${SERIALIZATIONOBJECTS} ${RUNTIMEOBJECTS} ${TRANSPORTOBJECTS} ${CLIENTOBJECTS} ${SERVEROBJECTS}

OUT=${OUTSERVER} ${OUTCLIENT}

all: ${OUT}

${OUTSERVER}: ${OUTSERVEROBJECTS}
	$(CC) -o $@ $^

${OUTCLIENT}: ${OUTCLIENTOBJECTS}
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cc 
	$(CC) -c $(INCLUDE) -o $@ $< $(CFLAGS)

# clean all .o object files and target binary files
clean:
	rm -f ${OUTSERVEROBJECTS} ${OUTCLIENTOBJECTS} ${OUT}
