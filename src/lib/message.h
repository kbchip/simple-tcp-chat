#pragma once

// Define some macro constants
#define MAX_TEXT_CHARS 64
#define MAX_USERNAME_CHARS 32

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
typedef struct MessageStruct
{
    enum MessageTypeCode type;
    char sender[ MAX_USERNAME_CHARS ];
    char receiver[ MAX_USERNAME_CHARS ];
    char text[ MAX_TEXT_CHARS ];
} Message;