#ifndef PRINT_PRE_DECREMENT_H
#define PRINT_PRE_DECREMENT_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_pre_decrement(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_PRE_DECREMENT_H