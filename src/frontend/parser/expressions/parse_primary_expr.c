#include <string.h>
#include "utils.h"
#include "frontend/parser/core.h"
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_primary_expr.h"

/**
 * @brief Parses a primary expression.
 *
 * This function is responsible for parsing the simplest building blocks of expressions, known as primary expressions.
 * These can include:
 * - Numeric literals (e.g., numbers).
 * - Identifiers (e.g., variable names).
 * - Parenthesized expressions (i.e., expressions within parentheses).
 * 
 * The function processes the token at the current parser position and attempts to match it to one of the expected 
 * primary expression types. If a valid primary expression is identified, it creates the corresponding AST node.
 *
 * @param parser A pointer to the `Parser` struct, which contains the tokens to be parsed and other related information.
 * @return An `AstNode` representing the parsed primary expression.
 *         If the token corresponds to a numeric literal, an identifier, or a valid parenthesized expression, 
 *         an AST node is created and returned. Otherwise, an error is raised.
 */
AstNode *parse_primary_expr(Parser *parser) {
    Token token = eat(parser);

    int line = at(parser).line;
    int column_start = at(parser).column_start;
    int column_end = at(parser).column_end;
    int position_start = at(parser).position_start;
    int position_end = at(parser).position_end;

    switch (token.type) {
        case TOKEN_NUMBER: {
            if (token.lexeme == NULL || strlen(token.lexeme) == 0) {
                error(parser, "Invalid number lexeme");
                return NULL;
            }

            NumericLiteralNode *numeric_data = MALLOC_S(sizeof(NumericLiteralNode));
            if (!numeric_data) {
                fprintf(stderr, "Error: Memory allocation failed for NumericLiteralNode\n");
                exit(EXIT_FAILURE);
            }

            if (strcmp(token.message, "decimal") == 0) {
                numeric_data->value = strtod(token.lexeme, NULL);
                numeric_data->decimal = true;
            } else {
                numeric_data->value = atoi(token.lexeme);
                numeric_data->decimal = false;
            }            

            AstNode *node = create_ast_node(
                NODE_NUMERIC_LITERAL,
                numeric_data,
                line, 
                column_start, 
                position_start, 
                column_end, 
                position_end
            );
            return node;
        }

        case TOKEN_IDENTIFIER: {
            if (token.lexeme == NULL || strlen(token.lexeme) == 0) {
                error(parser, "Invalid identifier lexeme");
                return NULL;
            }

            IdentifierNode *identifier_data = MALLOC_S(sizeof(IdentifierNode));
            if (!identifier_data) {
                fprintf(stderr, "Error: Memory allocation failed for IdentifierNode\n");
                exit(EXIT_FAILURE);
            }
            identifier_data->symbol = strdup(token.lexeme);

            AstNode *node = create_ast_node(
                NODE_IDENTIFIER,
                identifier_data,
                line, 
                column_start, 
                position_start, 
                column_end, 
                position_end
            );
            return node;
        }

        case TOKEN_OPAREN: {
            AstNode *node = parse_expr(parser);
            if (node == NULL) {
                error(parser, "Failed to parse expression inside parentheses");
                return NULL;
            }

            expect(parser, TOKEN_CPAREN, "Expected closing parenthesis");
            return node;
        }

        default:
            printf("Error token: %s\n", token.lexeme);
            error(parser, "Unexpected token in primary expression");
            return NULL;
    }
}

/**
 * @brief Condition to verify if the numbe is a Integer.
 * 
 * This functions verify if the number returned in TOKEN_NUMBER is a integer (decimal).
 * 
 * @param str: String to be scanned and converted.
 * @param isUnsigned: Verify if the number is a unsigned number (true or false).
 * @return value >= INT_MIN: Verify if the value is bigger or equal than the
 * minimal size of INTEGER value.
 */

// bool numberIsInteger(const char *str, bool *isUnsigned)
// {
//     char *End;
//     long value = strtol(str, &End, 10);

//     /**
//      * Return false if contains invalid characters.
//      */

//     if (*End != '\0') {
//         return false;
//     }

//     if (value >= 0) {
//         *isUnsigned = (value <= UINT_MAX);
//         return (value <= INT_MAX);
//     }

//     return (value >= INT_MIN);
// }

/**
 * @brief Verify if the number returned by the TOKEN_NUMBER is a float or double.
 * 
 * Verify if the number is a float or number. OBS: The double doesnt have the same
 * size of bits as float (float = 32, double = 64). With that, we assume that this functions
 * is not working properly, as we need to verify the size of both. 
 * 
 * fix(TODO):Verify if the size of float or double is 32 bits or 64 bits.
 * 
 * @param str: String to be scanned and converted.
 * @param isDouble: Verify if the number is a double value (64 bits).
 * @return true Return true if the value is a double value.
 */

// bool FloatOrDouble(const char *str, bool *isDouble) {
//     char *end;
//     double value = strdtod(str, &end);

//     if (*end != '\0') {
//         return false;
//     }

//     *isDouble = (value < FLT_MIN || value > FLT_MAX);
//     return true;
// }



// TokenType analyzeNumber(const char *number) {
//     bool isUnsigned = false;
//     bool isDouble = false;

//     if (isInteger(number, &isUnsigned)) {
//         if (isUnsigned) {
//             return (TokenType){TOKEN_NUMBER, "unsigned int"};
//         }

//         return (TokenType){TOKEN_NUMBER, "int"};
//     }
// }

