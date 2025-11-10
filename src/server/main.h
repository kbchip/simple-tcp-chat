#pragma once

// includes for server main file
#include "properties.h"
#include "message.h"
#include "chat_node.h"
#include "client_handler.h"
#include "dbg.h"

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