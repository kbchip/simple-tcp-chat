#include "chat_node.h"

ChatterNode* create_chatter_node();

ChatterNodeList* create_chatter_node_list();

ChatterNodeList* add_chatter_node(ChatterNodeList* list);

ChatterNodeList* remove_chatter_node(ChatterNodeList* list);

bool chatter_node_compare(ChatterNode left, ChatterNode right);