#include "main.h"
#include "properties.h"

int main() {
    char* properties_file = "server.properties";
    Properties* properties;
    char* key = "SERVER_PORT";
    char* value;

    properties = property_read_properties(properties_file);
    value = property_get_property(properties, key);

    printf("%s\n", value);
}