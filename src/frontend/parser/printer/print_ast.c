#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../../../include/ast/definitions.h"
#include "../../../../include/parser/printer/print_indent.h"
#include "../../../../include/parser/printer/visited.h"
#include "../../../../include/parser/printer/nodes/print_program.h"
#include "../../../../include/parser/printer/nodes/print_assignment.h"
#include "../../../../include/parser/printer/nodes/print_binary_expr.h"
#include "../../../../include/parser/printer/nodes/print_identifier.h"
#include "../../../../include/parser/printer/nodes/print_pre_decrement.h"
#include "../../../../include/parser/printer/nodes/print_property.h"
#include "../../../../include/parser/printer/nodes/print_package.h"
#include "../../../../include/parser/printer/print_ast.h"
#include "../../../../include/parser/printer/visited.h"


/**
 * @brief Returns the textual name of an AST node type.
 *
 * This function returns a string representing the name of the node type based on the 
 * enumerated value NodeType. It is useful for printing information about AST nodes.
 *
 * @param node_type The type of the AST node (enum NodeType).
 * @return A string with the name of the node type. Returns "Unknown" if the type is not recognized.
 */
const char* returnASTNodeName(NodeType node_type) {
    switch (node_type) {
        case NODE_PROGRAM: return "Program";
        case NODE_NUMERIC_LITERAL: return "Numeric Literal";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_BINARY_EXPR: return "Binary Expression";
        case NODE_PACKAGE: return "Package Statement";
        case NODE_IMPORT: return "Import Statement";
        case NODE_ASSIGNMENT: return "Assignment Expression";
        case NODE_OBJECT: return "Object Expression";
        case NODE_PROPERTY: return "Property";
        default: return "Unknown";
    }
}

/**
 * @brief Prints an AST node and its details.
 *
 * This function displays information about a specific AST node, including its type
 * and associated data. It uses indentation to reflect the node's depth in the AST structure.
 *
 * @param node The AST node to print.
 * @param depth The depth of the node in the AST, used for visual indentation.
 * @param visited Pointer to the VisitedNodes structure to track visited nodes.
 */
void print_ast_node(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || is_visited(visited, node)) return;

    mark_visited(visited, node);
    print_indent(depth);
    printf("Node Type: %s\n", returnASTNodeName(node->kind));

    switch (node->kind) {
        case NODE_PROGRAM: {
            print_program(node, depth, visited);
            break;
        }

        case NODE_ASSIGNMENT: {
            print_assignment(node, depth, visited);
            break;
        }

        case NODE_OBJECT: {
            print_object(node, depth, visited);
            break;
        }

        case NODE_PROPERTY: {
            print_property(node, depth, visited);
            break;
        }

        case NODE_PRE_DECREMENT: {
            print_pre_decrement(node, depth, visited);
            break;
        }

        case NODE_PACKAGE: {
            print_package(node, depth);
            break;
        }


        case NODE_IMPORT: {
            print_import(node, depth);
            break;
        }

        case NODE_NUMERIC_LITERAL: {
            print_numeric_literal(node, depth);
            break;
        }

        case NODE_IDENTIFIER: {
            print_identifier(node, depth);
            break;
        }

        case NODE_BINARY_EXPR: {
            print_binary_expr(node, depth, visited);
            break;
        }

        default:
            print_indent(depth + 1);
            printf("Value: Unknown or no data\n");
            break;
    }

    if (node->children) {
        for (size_t i = 0; i < node->child_count; i++) {
            if (node->children[i]) {
                print_ast_node(node->children[i], depth + 1, visited);
            } else {
                printf("Child %zu is NULL\n", i);
            }
        }
    }
}

/**
 * @brief Prints the entire AST structure.
 *
 * This function initiates the AST printing process by calling `print_ast_node` 
 * for the root node and managing the visited nodes structure.
 *
 * @param root The root node of the AST.
 */
void print_ast(const AstNode *root) {
    printf("AST:\n");
    VisitedNodes visited;
    init_visited(&visited);
    print_ast_node(root, 0, &visited);
    free_visited(&visited);
}
