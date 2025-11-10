// receiver_handler.c and receiver_handler.h - here you OUTLINE the code that is
//  being executed in the receiving thread: a function containing the server loop.
// In addition, you need to have another function in place that is being executed
// when the server connects in the server loop. You need to put a big switch statement
// into this latter function with entry points for the different kinds of messages
// that can be received from the server.

/*
 * receiver_handler.c
 * -------------------
 * Implementation of the receiver handler for the chat client.
 *
 * All functions are public—no static variables or static functions.
 * This design allows explicit control, testing, and extensibility.
 */

#include "receiver_handler.h"


// ===============================================================
// Receiver Handler Lifecycle
// ===============================================================

int receiver_handler_start(receiver_handler_t *handler, int socket_fd)
{
    if (handler == NULL)
        return -1;

    handler->server_socket_fd = socket_fd;
    handler->stop_requested = false;
    handler->running = true;

    if (pthread_create(&handler->thread_id, NULL, receiver_handler_thread_func, handler) != 0)
    {
        perror("[Receiver] Failed to create thread");
        handler->running = false;
        return -1;
    }

    return 0;
}

void receiver_handler_stop(receiver_handler_t *handler)
{
    if (handler == NULL || !handler->running)
        return;

    handler->stop_requested = true;

    // Unblock recv() if it's currently waiting
    shutdown(handler->server_socket_fd, SHUT_RD);

    pthread_join(handler->thread_id, NULL);
    handler->running = false;
}   

bool receiver_handler_is_running(receiver_handler_t *handler)
{
    if (handler == NULL)
        return false;

    return handler->running;
}

/*
TO DOS

implements:

receiver logic.
void *receiver_handler_thread_func(void *arg)

message processing (This will be the big switch statement).
void receiver_handler_process_message(struct MessageStruct *msg)

*/