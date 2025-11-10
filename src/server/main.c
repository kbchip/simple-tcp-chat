#include "main.h"

const char *const SERVER_PORT;
const unsigned int NUM_CONNECTIONS;
const unsigned int MAX_DATA_LENGTH;

int main() {
    char* const properties_file = "server.properties";
    Properties* properties;

    properties = property_read_properties(properties_file);
    const char *const SERVER_PORT = property_get_property(properties, "SERVER_PORT");
    const unsigned int NUM_CONNECTIONS = strtoul(
        property_get_property(properties, "NUM_CONNECTIONS"),
        NULL, 
        10
    );
    // const unsigned int MAX_DATA_LENGTH = strtoul(
    //     property_get_property(properties, "MAX_DATA_LENGTH"),
    //     NULL, 
    //     10
    // );

    int server_sock, client_sock;		// server and client socket descriptors
	struct addrinfo hints, *servinfo, *p;	// addrinfo structs (serverside)
	int gai_status;				// return code of getaddrinfo()
	struct sockaddr_storage client_addr;	// client address info
	int yes = 1;

	signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE sent when client disconnects
	
	// initialize serverside addrinfo
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use local (server) IP
	
	// handle and return if gai hits an error, otherwise resolve local hostname
	if ((gai_status = getaddrinfo(NULL, SERVER_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gai_status));
		return 1;
	}
	
	// loop through lookup results and bind to first possible
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((server_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		
		// remove "address already in use" error
		if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			perror("server: setsockopt");
			exit(1);
		}
		
		// try to bind to current lookup result
		if (bind(server_sock, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_sock);
			perror("server: bind");
			continue;
		}

		// if no error caught, continue with last valid socket
		break;
	}

	// if list of lookup results ends, return with error
	if (p == NULL) {
		fprintf(stderr, "server failed to bind\n");
		exit(1);
	}

	// servinfo no longer needed
	freeaddrinfo(servinfo);

	// try to listen with NUM_CONNECTIONS
	if (listen(server_sock, NUM_CONNECTIONS) == -1) {
		perror("listen");
		exit(1);
	}

	// server loop
	while (true) {
		// accept connection
		socklen_t addr_size = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
		// handle connection error
		if (client_sock == -1) {
			perror("accept");
			continue;
		}
		printf("\nServer with PID %d: accepted client\n", getpid());

		// create thread to handle client (naive approach, does not account for race conditions)
		pthread_t thread;
		if (pthread_create(&thread, NULL, handle_client, (void*)&client_sock) != 0) {
			perror("pthread: creating thread");
			exit(1);
		}

		// detatch thread so memory is reclaimed as thread finishes
		if (pthread_detach(thread) != 0) {
			perror("pthread: detach");
			exit(1);
		}
	}
}