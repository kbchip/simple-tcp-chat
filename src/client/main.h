#pragma once

// Some handy macros
#define CLIENT_PROPERTIES_FILENAME "client.properties"


// Networking libraries
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// includes for server main file
#include "properties.h"
#include "message.h"
#include "chat_node.h"
#include "dbg.h"

// Include client libraries
#include "sender_handler.h"
#include "receiver_handler.h"


// std libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>