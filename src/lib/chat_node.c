#include "chat_node.h"

ChatterNode* create_chatter_node();

ChatterNodeList* create_chatter_node_list();

ChatterNodeList* add_chatter_node(ChatterNodeList* list, ChatterNode* node);

ChatterNodeList* remove_chatter_node(ChatterNodeList* list, ChatterNode* node);

ChatterNodeList* remove_chatter_by_username(ChatterNodeList* list, char *username);

bool chatter_node_compare(ChatterNode *left, ChatterNode *right);