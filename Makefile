# Makefile created with the assistance of Microsoft Copilot

CFLAGS := -Wall -Isrc/lib -MMD -MP
LDFLAGS := -pthread -lpthread

DEBUG ?= 0
ifeq ($(DEBUG),1)
CFLAGS += -ggdb -DDEBUG -Wextra
endif

CLIENT_SRC := $(wildcard src/client/*.c)
SERVER_SRC := $(wildcard src/server/*.c)
LIB_SRC := $(wildcard src/lib/*.c)

CLIENT_OBJ := $(CLIENT_SRC:src/%.c=build/%.o)
SERVER_OBJ := $(SERVER_SRC:src/%.c=build/%.o)
LIB_OBJ := $(LIB_SRC:src/%.c=build/%.o)
LIB_STATIC := $(LIB_SRC:src/%c=build/%_lib.a)

CLIENT_BIN := build/chat_client
SERVER_BIN := build/chat_server

all: client server

client: $(CLIENT_BIN)
server: $(SERVER_BIN)

$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $@ $^

$(CLIENT_BIN): $(CLIENT_OBJ) $(LIB_STATIC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	cp src/client/*.properties build/

$(SERVER_BIN): $(SERVER_OBJ) $(LIB_STATIC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	cp src/server/*.properties build/

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf build

.PHONY: all client server clean

-include build/**/*.d
