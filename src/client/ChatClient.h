#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

// Include inportant libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Networking libraries
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Define some macro constants
#define MAX_TEXT_CHARS 64
#define MAX_USERNAME_CHARS 32

// For specific int sizes
#include <stdint.h>

// Defines the type of message that is being sent
enum MessageTypeCode
{
    JOIN,
    LEAVE,
    NOTE,
    SHUTDOWN,
    SHUTDOWN_ALL,
    DIRECT_MESSAGE
};


// Struct for sent message
struct MessageStruct
{
    enum MessageTypeCode type;
    char sender[ MAX_USERNAME_CHARS ];
    char receiver[ MAX_USERNAME_CHARS ];
    char text[ MAX_TEXT_CHARS ];
} Message;

// Function prototype definitions


#endif
