// main.c and main.h - main.c contains the detailed code that is kicking off a
// client - reading properties_list and then starting the two threads that implement
// both the sending and the receiving side with respect to the server. This code
// needs to be fully functional and should only need little tweaking as a result
// of debugging.

#include "main.h"
#include "properties.h"
#include "receiver_handler.h"
#include "sender_handler.h"

int main( int argc, char *argv[] ) {
    //// Initialize variables
    bool found_valid_socket = false;  // Flag the track the status of finding a valid socket
    int gai_status;                  // return code of getaddrinfo()
	int client_socket;		        // client socket from socket()

    // Sender and receiver structs
    sender_thread_args_t sender_arguments;
    receiver_handler_t receiver_arguments;

    // Threads
    pthread_t sender_thread_holder;
    pthread_t receiver_thread_holder;

    // Mutex lock varaibles to track exit status of threads
    pthread_mutex_t thread_shutdown_lock;  // Lock to prevent status updates race conditions
    bool is_thread_shutdown;       // Used to check if a thread has been shutdown, true if so 

	// create addrinfo structs
	struct addrinfo hints;          // Arguments connection type needed
    struct addrinfo *server_info;    // Actual returned server info
    struct addrinfo *current_server_info; // Current working server info

	// initialize addrinfo
	memset(&hints, 0, sizeof(hints)); 	// ensure struct is empty
	hints.ai_family = AF_UNSPEC;		// IPvX doesn't matter
	hints.ai_socktype = SOCK_STREAM;	// TCP stream
    Properties *properties_list;

    // Print start message
    printf( "-- Simple TCP Chat Start --" );

    // Get data from properties file and add to struct
    properties_list = property_read_properties( CLIENT_PROPERTIES_FILENAME );

    // Read needed properties
    const char *const SERVER_PORT = property_get_property( properties_list, "SERVER_PORT" );
    const char* const SERVER_ADDRESS = property_get_property( properties_list, "SERVER_ADDRESS" );


    //// Initiate connection to server
	
	// handle and return if gai hits an error, otherwise resolve DNS/IP
	if ( ( gai_status = getaddrinfo(SERVER_ADDRESS, SERVER_PORT, &hints, &server_info ) ) != 0)
    {
        // Error in getting the address info
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gai_status));
		return 1;
	}
	
	// Loop through lookup results and connect to first working socket
    while( !found_valid_socket && current_server_info != NULL )
    {	
        // attempt to get client socket and handle error
		if ( ( client_socket = socket(current_server_info->ai_family, current_server_info->ai_socktype, current_server_info->ai_protocol)) == -1 )
        {
			perror("client: socket");
		}
        // attempt to connect to socket and handle error
        else if ( connect( client_socket, current_server_info->ai_addr, current_server_info->ai_addrlen) == -1)
        {
			perror("client: connect");
			close(client_socket);
		}
        else
        {
            // We found a valid socket, set flag
            found_valid_socket = true;
        }
        
        // Move to next server info
        current_server_info = current_server_info->ai_next;
    }
	// Exited loop to find valid socket


	// Now that we are connected, we no longer need the server info
	freeaddrinfo(server_info);

    // Check if we found a valid socket and if so, start handler threads!
    if( found_valid_socket )
    {
        // initialize mutex locks
        pthread_mutex_init( &thread_shutdown_lock, NULL );
        is_thread_shutdown = false;

        // Build the struct to hold sender arguemnts
        sender_arguments.server_socket = client_socket;
        sender_arguments.thread_shutdown_lock = &thread_shutdown_lock;
        sender_arguments.is_thread_shutdown = &is_thread_shutdown;

        // Build the struct to hold receiver arguemnts
        receiver_arguments.server_socket = client_socket;
        receiver_arguments.thread_shutdown_lock = &thread_shutdown_lock;
        receiver_arguments.is_thread_shutdown = &is_thread_shutdown;

        // Start sending handler
        if (pthread_create(&sender_thread_holder, NULL,
                                sender_thread, (void*)&sender_arguments) != 0) {
			perror("pthread: sender thread creation");
			exit(1);
		}
        // Separate receiving handler
        else if (pthread_create(&receiver_thread_holder, NULL,
                                    receiver_handler_thread_func, (void*)&receiver_arguments) != 0) {
            perror("pthread: receiver thread creation");
			exit(1);
        }
        
        // Wait for both threads to complete
        pthread_join( sender_thread_holder, NULL );
        pthread_join( receiver_thread_holder, NULL );

        // Threads finished, close socket
        close( client_socket );
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
}