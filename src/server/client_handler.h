/**
 * client_handler.h
 * -----------------
 * Header file for the server-side client handler module.
 * 
 * Each connected client is handled in its own thread, running the 
 * function `handle_client(void* arg)`. This thread receives messages 
 * from the client, interprets them (JOIN, NOTE, LEAVE, SHUTDOWN ALL), 
 * and performs the appropriate server-side actions.
 * 
 * The corresponding implementation is in client_handler.c.
 */

#pragma once

#include "message.h"     // For message structure and message types
#include "chat_node.h"   // For chat node structure and client list
#include <pthread.h>     // For threading (pthread_t)
#include <netinet/in.h>  // For socket address structures
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dbg.h"

/**
 * Struct passed to each client thread.
 * Contains the socket file descriptor and any other
 * relevant information needed to communicate with the client.
 */
typedef struct {
    int client_socket;             // Connected client's socket descriptor
    struct sockaddr_in client_addr; // Client's network address
} client_thread_args_t;

/**
 * Thread entry point for handling a single client connection.
 * 
 * This function:
 *  - Continuously listens for messages from the client.
 *  - Uses a switch-case to process message types (JOIN, NOTE, LEAVE, etc.).
 *  - Updates the server’s list of connected clients as needed.
 *  - Broadcasts messages to other clients when appropriate.
 *  - Cleans up when the client disconnects.
 */
void* handle_client(void* arg);

extern const char *const SERVER_PORT;
extern const unsigned int NUM_CONNECTIONS;
extern const unsigned int MAX_DATA_LENGTH;