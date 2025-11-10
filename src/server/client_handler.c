#include "client_handler.h"

void* handle_client(void* arg) {
	int client_sock = *((int*)arg);	// connected socket, taken from accept loop
    Message message;

    // loop until LEAVE message received

        switch (message.type) {
            case JOIN:
                // check if username is already taken
                    // if not, add user to list and proceed
                    // if it is, inform client and immediately terminate connection
            case LEAVE:
                // terminate connection
            case NOTE:
                // iterate over list of all users that are not the sender
                    // relay message contents
            case SHUTDOWN:
                // terminate connection, rest of SHUTDOWN is handle on client
            case SHUTDOWN_ALL:
                // immediately terminate all connections and exit with special retval
            case DIRECT_MESSAGE:
                // relay message contents to specified user
        }

	// cleanup!
	if (close(client_sock) == -1) {
		perror("error closing socket");
		exit(1);
	} else {
		printf("Closed socket to client, exiting.");
	}

	pthread_exit(NULL);
}