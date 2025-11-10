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

/*
TO DOS

implements:

receiver logic.
void *receiver_handler_thread_func(void *arg)

message processing (This will be the big switch statement).
void receiver_handler_process_message(struct MessageStruct *msg)

*/

void *receiver_handler_thread_func(void *arg)
{
    receiver_handler_t *handler = (receiver_handler_t *)arg;
    if (handler == NULL) {
        pthread_exit(NULL);
    }

    int sockfd = handler->server_socket;
    Message msg;

    printf("[Client] Receiver thread started.\n");

    /*
     * Skeleton loop:
     * - recv() a fixed-size Message struct
     * - on success, print a short representation to stdout
     * - on error or disconnect, break and exit the thread
     *
     * This is intentionally minimal; you can expand it to handle
     * partial reads, better error handling, and a full switch on
     * message types later.
     */
    while (1) {
        /* Check shared shutdown flag before blocking on recv() */
        if (handler->thread_shutdown_lock != NULL && handler->is_thread_shutdown != NULL) {
            pthread_mutex_lock(handler->thread_shutdown_lock);
            bool should_shutdown = *(handler->is_thread_shutdown);
            pthread_mutex_unlock(handler->thread_shutdown_lock);

            if (should_shutdown) {
                /* Other thread requested shutdown; exit loop */
                printf("[Client] Receiver detected shutdown signal.\n");
                break;
            }
        }

        ssize_t n = recv(sockfd, &msg, sizeof(msg), 0);

        if (n == 0) {
            /* connection closed by peer */
            printf("[Client] Server closed connection.\n");
            break;
        } else if (n < 0) {
            if (errno == EINTR)
                continue;
            perror("[Client] recv");
            break;
        }

        // Display message
        receiver_handler_process_message( &msg );
        
    }

    printf("[Client] Receiver thread exiting.\n");
    pthread_exit(NULL);
}

void receiver_handler_process_message(struct MessageStruct *msg) {
    /* Basic display of received message */
    switch (msg->type) {
        case JOIN:
            printf("[Server] %s joined.\n", msg->sender);
            break;
        case LEAVE:
            printf("[Server] %s left.\n", msg->sender);
            break;
        case NOTE:
            printf("[%s] %s\n", msg->sender, msg->text);
            break;
        case DIRECT_MESSAGE:
            printf("[DM from %s] %s\n", msg->sender, msg->text);
            break;
        case SHUTDOWN:
            printf("[Server] Shutdown requested.\n");
            break;
        case SHUTDOWN_ALL:
            printf("[Server] Shutdown all requested.\n");
            break;
        default:
            printf("[Server] Unknown message type %d\n", (int)msg->type);
            break;
    }
}