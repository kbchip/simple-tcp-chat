#pragma once


// TODO: May need this library included, but I'm not sure yet.
// #include <stdint.h>
#include <sys/types.h>

#include "message.h"

//  these define the structure that represents the identifying information of a
// chat node, i.e. IP, port and logical name. You will also want to foresee
// functionality that acts upon chat nodes. One important example is to have code
// in place that implements linked lists for chat nodes. This code needs to be
// OUTLINED.

typedef struct ChatterNodeStruct {
    u_int32_t ipAddress;
    u_int8_t portNumber;
    char username[ MAX_USERNAME_CHARS ];
} ChatterNode;

struct ChatterNodeList {
    struct ChatterNodeStruct *nextChatter;
};