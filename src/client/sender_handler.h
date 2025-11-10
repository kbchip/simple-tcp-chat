/*
sender_handler.h

This module defines the interface for the thread that handles
all user input.

the sender thread continuously reads user commands or text from stdin,
interprets them (JOIN, LEAVE, NOTE, SHUTDOWN, SHUTDOWN ALL),
and sends appropriate messages to the chat server.

The sender runs concurrently with the receiver thread, which listens for 
messages from the server.
*/

// Guards
#pragma once

#include "message.h"      // Message struct and const (JOIN, NOTE, etc)
#include <pthread.h>
#include <netinet/in.h>   // for sockaddr_in
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/*
Struct used to pass information from the main client thread
to the sender thread at startup
*/
typedef struct {
    int server_socket; // Connected TCP socket to the chat server
    pthread_mutex_t *thread_shutdown_lock;  // Lock to prevent status updates race conditions
    bool *is_thread_shutdown;       // Used to check if a thread has been shutdown, true if so 
} sender_thread_args_t;


// Function prototypes
void* sender_thread(void* arg);