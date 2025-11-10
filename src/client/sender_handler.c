/*
sender_handler.c

CLIENT SIDE

Implementation of client-side sender handler functions

Continously listen for user input, interprets commands,
builds message structures, and sends them to the server.

TO DO's
1. Implement actual MESSAGE struct creation and SEND() logic
2. Add state validation (prevent sending NOTES before JOIN)
3. Handle socket connection setup (on JOIN)
4. Add graceful shutdown coordination with main thread
*/

#include "sender_handler.h"

void* sender_thread(void* arg)
{
    sender_thread_args_t* args = (sender_thread_args_t*)arg;
    int server_socket = args->server_socket;

    char input_buffer[512];
    char command[32];
    int running = 1;   // Control flag for main loop

    printf("[Client] Sender thread started. Type a command or message.\n");

    // Main input loop
    while (running) {
        printf("> ");
        fflush(stdout);

        // Read user input
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("[Client] Input error or EOF detected. Exiting sender thread.\n");
            running = 0;
        } 
        else {
            // Remove trailing newline
            input_buffer[strcspn(input_buffer, "\n")] = '\0';

            // If the user entered nothing, skip processing this cycle
            if (strlen(input_buffer) == 0) {
                // Explicitly handle empty input without continue
            } 
            else {
                // Extract first word to identify potential command
                sscanf(input_buffer, "%31s", command);

                // Convert command to uppercase
                for (int i = 0; command[i]; i++) {
                    if (command[i] >= 'a' && command[i] <= 'z')
                        command[i] -= 32;
                }

                /**
                 * Command Switch
                 * --------------
                 * Identify and process user commands.
                 * Anything not matching a known command is treated as NOTE.
                 */
                enum { CMD_JOIN, CMD_LEAVE, CMD_SHUTDOWN, CMD_SHUTDOWN_ALL, CMD_NOTE } cmd_type;

                if (strcmp(command, "JOIN") == 0)
                    cmd_type = CMD_JOIN;
                else if (strcmp(command, "LEAVE") == 0)
                    cmd_type = CMD_LEAVE;
                else if (strcmp(command, "SHUTDOWN") == 0 && strstr(input_buffer, "ALL") != NULL)
                    cmd_type = CMD_SHUTDOWN_ALL;
                else if (strcmp(command, "SHUTDOWN") == 0)
                    cmd_type = CMD_SHUTDOWN;
                else
                    cmd_type = CMD_NOTE;

                switch (cmd_type) {

                    case CMD_JOIN:
                        printf("[Client] Detected JOIN command.\n");
                        // TODO:
                        //   - Parse IP/port and attempt connection.
                        //   - Send MSG_JOIN to server.
                        //   - Update args->is_connected = true.
                        break;

                    case CMD_LEAVE:
                        printf("[Client] Detected LEAVE command.\n");
                        // TODO:
                        //   - Check if connected.
                        //   - Send MSG_LEAVE to server.
                        //   - Update args->is_connected = false.
                        break;

                    case CMD_SHUTDOWN:
                        printf("[Client] Detected SHUTDOWN command.\n");
                        // TODO:
                        //   - If connected, send MSG_LEAVE first.
                        //   - Close socket and clean up.
                        //   - End this thread.
                        running = 0; // graceful exit after switch
                        break;

                    case CMD_SHUTDOWN_ALL:
                        printf("[Client] Detected SHUTDOWN ALL command.\n");
                        // TODO:
                        //   - Ask for user confirmation.
                        //   - If confirmed, send MSG_SHUTDOWN_ALL to server.
                        //   - Exit client cleanly.
                        running = 0;
                        break;

                    case CMD_NOTE:
                        printf("[Client] Sending NOTE message: %s\n", input_buffer);
                        // TODO:
                        //   - Ensure client is connected.
                        //   - Build MSG_NOTE message and send to server.
                        break;
                } // end switch
            } // end else (non-empty input)
        } // end else (fgets success)
    } // end while

    printf("[Client] Sender thread exiting.\n");
    pthread_exit(NULL);
}