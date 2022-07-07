# Specific part of the Makefile
EXEC_CLIENT=client
EXEC_SERVER=server

CC=gcc	# compilateur
CFLAGS=-Werror # options compilateur
LDFLAGS=	# edition de lien

SRC_CLIENT = client.c
SRC_SERVER = server.c List/list.c Queue/queue.c
OBJ_CLIENT= $(SRC_CLIENT:.c=.o)
OBJ_SERVER= $(SRC_SERVER:.c=.o)

ifeq ($(DEBUG),yes)	#mode debug=yes
	CFLAGS += -g
	LDFLAGS =
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS =
endif

#not to be confused with clean files or mrproprer if they exist
.PHONY: clean mrproper

#to make all
all: $(EXEC_CLIENT) $(EXEC_SERVER)


%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC_CLIENT): $(OBJ_CLIENT)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_SERVER): $(OBJ_SERVER)
	@$(CC) -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -f $(EXEC_SERVER)
	@rm -f $(EXEC_CLIENT)

#doc: $(DOC)
#	@doxygen ../Documentation/dconfig
