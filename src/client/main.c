// main.c and main.h - main.c contains the detailed code that is kicking off a
// client - reading propertiesList and then starting the two threads that implement
// both the sending and the receiving side with respect to the server. This code
// needs to be fully functional and should only need little tweaking as a result
// of debugging.

#include "main.h"
#include "properties.h"
#include "receiver_handler.h"
#include "sender_handler.h"

int main(int argc, char *argv[]) {
    //// Initialize variables
    bool foundValidSocket = false;  // Flag the track the status of finding a valid socket
    int gaiStatus;                  // return code of getaddrinfo()
	int clientSocket;		        // client socket from socket()

    // Sender and receiver structs
    sender_thread_args_t senderArguments;
    receiver_handler_t receiverArguments;

	// create addrinfo structs
	struct addrinfo hints;          // Arguments connection type needed
    struct addrinfo *serverInfo;    // Actual returned server info
    struct addrinfo *currentServerInfo; // Current working server info

	// initialize addrinfo
	memset(&hints, 0, sizeof(hints)); 	// ensure struct is empty
	hints.ai_family = AF_UNSPEC;		// IPvX doesn't matter
	hints.ai_socktype = SOCK_STREAM;	// TCP stream
    Properties *propertiesList;

    // Print start message
    printf( "-- Simple TCP Chat Start --" );

    // Get data from properties file and add to struct
    propertiesList = property_read_properties( CLIENT_PROPERTIES_FILENAME );

    // Read needed properties
    const char *const SERVER_PORT = property_get_property(propertiesList, "SERVER_PORT");
    const char* const SERVER_ADDRESS = property_get_property( propertiesList, "SERVER_ADDRESS" );


    //// Initiate connection to server
	
	// handle and return if gai hits an error, otherwise resolve DNS/IP
	if ( ( gaiStatus = getaddrinfo(SERVER_ADDRESS, SERVER_PORT, &hints, &serverInfo ) ) != 0)
    {
        // Error in getting the address info
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gaiStatus));
		return 1;
	}
	
	// loop through lookup results and connect to first working socket
    while( !foundValidSocket && currentServerInfo != NULL )
    {	
        // attempt to get client socket and handle error
		if ( ( clientSocket = socket(currentServerInfo->ai_family, currentServerInfo->ai_socktype, currentServerInfo->ai_protocol)) == -1 )
        {
			perror("client: socket");
		}
        // attempt to connect to socket and handle error
        else if ( connect( clientSocket, currentServerInfo->ai_addr, currentServerInfo->ai_addrlen) == -1)
        {
			perror("client: connect");
			close(clientSocket);
		}
        else
        {
            // We found a valid socket, set flag
            foundValidSocket = true;
        }
        
        // Move to next server info
        currentServerInfo = currentServerInfo->ai_next;
    }
	// Exited loop to find valid socket

	// Now that we are connected, we no longer need the 
	freeaddrinfo(serverInfo);

    // Check if we found a valid socket and if so, start handler threads!
    if( foundValidSocket )
    {
        // Start sending handler
            // Give it the propertiesList struct


        
        // Start receiving handler
            // Give it the propertiesList struct
            // server loop

        // Wait for both threads to complete

        // Threads finished, close socket
        close( clientSocket );

    }
    else
    {
        fprintf(stderr, "client failed to connect\n");
        return 2;
	}

    // Print exit message
    printf( "-- Program end --" );

    // Exit
	return 0;

	while (true) {
		// accept connection
		socklen_t addr_size = sizeof(client_addr);
		clientSocket = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
		// handle connection error
		if (clientSocket == -1) {
			perror("accept");
			continue;
		}
		printf("\nServer with PID %d: accepted client\n", getpid());

		// create thread to handle client (naive approach, does not account for race conditions)
		pthread_t thread;
		if (pthread_create(&thread, NULL, handle_client, (void*)&clientSocket) != 0) {
			perror("pthread: creating thread");
			exit(1);
		}

		// detatch thread so memory is reclaimed as thread finishes
		if (pthread_detach(thread) != 0) {
			perror("pthread: detach");
			exit(1);
		}
	}
    // Print exit message
}