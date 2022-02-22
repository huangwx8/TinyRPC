CC=g++
CFLAGS=-g -std=c++11 -lpthread -Wall
SERVERBUILDFLAGS=-DBUILD_SERVER
CLIENTBUILDFLAGS=-DBUILD_CLIENT

INCLUDE=-I${PWD}/include
SRCDIR=${PWD}/source
OBJDIR=${PWD}/obj
OUTDIR=${PWD}/bin

COMMONOBJECTS=$(patsubst ${SRCDIR}/common/%.cc,${OBJDIR}/common/%.o,$(wildcard ${SRCDIR}/common/*.cc))
SCOMMONOBJECTS=$(patsubst %.o,%.server.o,${COMMONOBJECTS})
CCOMMONOBJECTS=$(patsubst %.o,%.client.o,${COMMONOBJECTS})

SERIALIZATIONOBJECTS=$(patsubst ${SRCDIR}/serialization/%.cc,${OBJDIR}/serialization/%.o,$(wildcard ${SRCDIR}/serialization/*.cc))
SSERIALIZATIONOBJECTS=$(patsubst %.o,%.server.o,${SERIALIZATIONOBJECTS})
CSERIALIZATIONOBJECTS=$(patsubst %.o,%.client.o,${SERIALIZATIONOBJECTS})

RUNTIMEOBJECTS=$(patsubst ${SRCDIR}/runtime/%.cc,${OBJDIR}/runtime/%.o,$(shell find ${SRCDIR}/runtime/ -name '*.cc'))
SRUNTIMEOBJECTS=$(patsubst %.o,%.server.o,${RUNTIMEOBJECTS})
CRUNTIMEOBJECTS=$(patsubst %.o,%.client.o,${RUNTIMEOBJECTS})

TRANSPORTOBJECTS=$(patsubst ${SRCDIR}/transport/%.cc,${OBJDIR}/transport/%.o,$(wildcard ${SRCDIR}/transport/*.cc))
STRANSPORTOBJECTS=$(patsubst %.o,%.server.o,${TRANSPORTOBJECTS})
CTRANSPORTOBJECTS=$(patsubst %.o,%.client.o,${TRANSPORTOBJECTS})

SERVEROBJECTS=$(patsubst ${SRCDIR}/server/%.cc,${OBJDIR}/server/%.server.o,$(wildcard ${SRCDIR}/server/*.cc)) ${OBJDIR}/example/NameService.server.o ${OBJDIR}/example/NameServer.server.o
CLIENTOBJECTS=$(patsubst ${SRCDIR}/client/%.cc,${OBJDIR}/client/%.client.o,$(wildcard ${SRCDIR}/client/*.cc)) ${OBJDIR}/example/NameService.client.o ${OBJDIR}/example/NameClient.client.o

OUTSERVER=${OUTDIR}/Server
OUTCLIENT=${OUTDIR}/Client
OUTSERVEROBJECTS=${OBJDIR}/example/Server.server.o ${SCOMMONOBJECTS} ${SSERIALIZATIONOBJECTS} ${SRUNTIMEOBJECTS} ${STRANSPORTOBJECTS} ${SERVEROBJECTS}
OUTCLIENTOBJECTS=${OBJDIR}/example/Client.client.o ${CCOMMONOBJECTS} ${CSERIALIZATIONOBJECTS} ${CRUNTIMEOBJECTS} ${CTRANSPORTOBJECTS} ${CLIENTOBJECTS}

server: ${OUTSERVER}

client: ${OUTCLIENT}

${OUTSERVER}: ${OUTSERVEROBJECTS}
	@echo '****************************************************************'
	@echo 'linking target file [${OUTSERVER}]...'
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo '****************************************************************'

${OUTCLIENT}: ${OUTCLIENTOBJECTS}
	@echo '****************************************************************'
	@echo 'linking target file [${OUTCLIENT}]...'
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo '****************************************************************'

$(OBJDIR)/%.server.o: $(SRCDIR)/%.cc 
	@echo 'compiling server c++ file $<'
	@$(CC) -c $(INCLUDE) -o $@ $< $(CFLAGS) $(SERVERBUILDFLAGS)

$(OBJDIR)/%.client.o: $(SRCDIR)/%.cc 
	@echo 'compiling client c++ file $<'
	@$(CC) -c $(INCLUDE) -o $@ $< $(CFLAGS) $(CLIENTBUILDFLAGS)

# clean all .o object files
clean:
	@echo 'clean all object files...'
	@rm -f ${OUTSERVEROBJECTS} ${OUTCLIENTOBJECTS}
