/*
receiver_handler.h

Defines the receiver handler for the client application.

Handles all messages incoming from the server.

It runs in a separate thread to while sender_handler runs user input

*/

#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for close(), shutdown()
#include <errno.h>
#include <arpa/inet.h>  // for recv()

#include "message.h"

typedef struct {
    int server_socket;       // Connected socket
    pthread_mutex_t *thread_shutdown_lock;  // Lock to prevent status updates race conditions
    bool *is_thread_shutdown;       // Used to check if a thread has been shutdown, true if so 
} receiver_handler_t;


// Continuously reads messages from server and passes them 
// to receiver_handler_process_message()
void *receiver_handler_thread_func(void *arg);

// Called internally by the receiver thread for each complete message
void receiver_handler_process_message(struct MessageStruct *msg);


