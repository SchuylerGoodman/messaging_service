# Makefile for message client and server

CXX=			g++ $(CCFLAGS)

MESSAGE-SERVER=		message-server.o inet-server.o server.o message.o command.o
MESSAGE-CLIENT=		message-client.o inet-client.o client.o request.o
OTHER=				util.o

OBJS =			$(MESSAGE-SERVER) $(MESSAGE-CLIENT) $(OTHER)

LIBS=

CCFLAGS= -g -std=c++11

all:	message-server message-client

message-server:$(MESSAGE-SERVER) $(OTHER)
	$(CXX) -o msgd $(MESSAGE-SERVER) $(OTHER) $(LIBS)

message-client:$(MESSAGE-CLIENT) $(OTHER)
	$(CXX) -o msg $(MESSAGE-CLIENT) $(OTHER) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) msgd msg


# These lines ensure that dependencies are handled automatically.
%.d:	%.cpp
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
