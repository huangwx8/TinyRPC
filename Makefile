CC=g++
CFLAGS=-g -std=c++11 -lpthread

INCLUDE=-I${PWD}/include
SRCDIR=${PWD}/source
OBJDIR=${PWD}/obj
OUTDIR=${PWD}/bin

COMMONOBJECTS=$(patsubst ${SRCDIR}/common/%.cc,${OBJDIR}/common/%.o,$(wildcard ${SRCDIR}/common/*.cc))
SERIALIZATIONOBJECTS=$(patsubst ${SRCDIR}/serialization/%.cc,${OBJDIR}/serialization/%.o,$(wildcard ${SRCDIR}/serialization/*.cc))
RUNTIMEOBJECTS=$(patsubst ${SRCDIR}/runtime/%.cc,${OBJDIR}/runtime/%.o,$(shell find ${SRCDIR}/runtime/ -name '*.cc'))
TRANSPORTOBJECTS=$(patsubst ${SRCDIR}/transport/%.cc,${OBJDIR}/transport/%.o,$(wildcard ${SRCDIR}/transport/*.cc))
SERVEROBJECTS=$(patsubst ${SRCDIR}/server/%.cc,${OBJDIR}/server/%.o,$(wildcard ${SRCDIR}/server/*.cc))
CLIENTOBJECTS=$(patsubst ${SRCDIR}/client/%.cc,${OBJDIR}/client/%.o,$(wildcard ${SRCDIR}/client/*.cc))

OUTSERVER=${OUTDIR}/EchoServer
OUTCLIENT=${OUTDIR}/EchoClient
OUTSERVEROBJECTS=${OBJDIR}/example/EchoServer.o ${COMMONOBJECTS} ${SERIALIZATIONOBJECTS} ${RUNTIMEOBJECTS} ${TRANSPORTOBJECTS} ${SERVEROBJECTS}
OUTCLIENTOBJECTS=${OBJDIR}/example/EchoClient.o ${COMMONOBJECTS} ${SERIALIZATIONOBJECTS} ${RUNTIMEOBJECTS} ${TRANSPORTOBJECTS} ${CLIENTOBJECTS}

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

$(OBJDIR)/%.o: $(SRCDIR)/%.cc 
	@echo 'compiling c++ file $<'
	@$(CC) -c $(INCLUDE) -o $@ $< $(CFLAGS) $(BUILDFLAGS)

# clean all .o object files
clean:
	@echo 'clean all object files...'
	@rm -f ${OUTSERVEROBJECTS} ${OUTCLIENTOBJECTS}
