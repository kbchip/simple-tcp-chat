/*
receiver_handler.h

Defines the receiver handler for the client application.

Handles all messages incoming from the server.

It runs in a separate thread to while sender_handler runs user input

*/

#pragma once

#include <pthread.h>
#include <stdbool.h>
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for close(), shutdown()
#include <errno.h>
#include <arpa/inet.h>  // for recv()

typedef struct {
    pthread_t thread_id;        // Receiver thread handle
    int server_socket_fd;       // Connected socket
    bool running;               // Is thread active
    bool stop_requested;        // Has stop been requested
} receiver_handler_t;

// Starts the receiver thread (initializes fields as needed)
int receiver_handler_start(receiver_handler_t *handler, int socket_fd);

// Requests the receiver thread to stop
void receiver_handler_stop(receiver_handler_t *handler);

// Checks if the receiver thread is active
bool receiver_handler_is_running(receiver_handler_t *handler);

// Continuously reads messages from server and passes them 
// to receiver_handler_process_message()
void *receiver_handler_thread_func(void *arg);

// Called internally by the receiver thread for each complete message
void receiver_handler_process_message(struct MessageStruct *msg);


