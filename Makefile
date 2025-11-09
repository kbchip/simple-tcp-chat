# Makefile created with the assistance of Microsoft Copilot

CFLAGS := -Wall -Isrc/lib -MMD -MP
LDFLAGS := -pthread -lpthread

DEBUG ?= 0
ifeq ($(DEBUG),1)
CFLAGS += -ggdb -DDEBUG -Wextra
endif

CLIENT_SRC := $(wildcard src/client/*.c)
SERVER_SRC := $(wildcard src/server/*.c)
LIB_SRC := src/lib/properties.c

CLIENT_OBJ := $(CLIENT_SRC:src/%.c=build/%.o)
SERVER_OBJ := $(SERVER_SRC:src/%.c=build/%.o)
LIB_OBJ := build/lib/properties.o
LIB_STATIC := build/lib/libproperties.a

CLIENT_BIN := build/chat_client
SERVER_BIN := build/chat_server

all: client server

client: $(CLIENT_BIN)
server: $(SERVER_BIN)

$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $@ $^

$(CLIENT_BIN): $(CLIENT_OBJ) $(LIB_STATIC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SERVER_BIN): $(SERVER_OBJ) $(LIB_STATIC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf build

.PHONY: all client server clean

-include build/**/*.d
