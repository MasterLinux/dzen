#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef DZEN_DATA_TYPES
#define DZEN_DATA_TYPES


#define YYSTYPE struct dzen_token
#define YY_INPUT(buf, result, max_size)        \
                  {                                              \
                    int yyc= get_next_parser_input_char();       \
                    result= (EOF == yyc) ? 0 : (*(buf)= yyc, 1); \
                  }

/**
* Clones a c string and returns it
*
* @param srcStr The c string to clone
* @return The clone of the given c string
*/
char *clone_string(char *src_string);

/**
* Collection of each existing token type
*/
typedef enum dzen_token_type {
    DZEN_TOKEN_TYPE_UNDEFINED,
    DZEN_TOKEN_TYPE_ROOT,
    DZEN_TOKEN_TYPE_TEXT,
    DZEN_TOKEN_TYPE_FILE_PATH,
    DZEN_TOKEN_TYPE_FUNCTION,
    DZEN_TOKEN_TYPE_CONSTANT,
    DZEN_TOKEN_TYPE_NUMBER,
    DZEN_TOKEN_TYPE_SIZE_PARAMETER,
    DZEN_TOKEN_TYPE_POSITIONING_PARAMETER,
    DZEN_TOKEN_TYPE_POSITIONING_PARAMETER_LIST,
    DZEN_TOKEN_TYPE_COLOR_KEYWORD,
    DZEN_TOKEN_TYPE_HEX_COLOR,
    DZEN_TOKEN_TYPE_NAME_IDENTIFIER,
    DZEN_TOKEN_TYPE_PLUS_OPERATOR,
    DZEN_TOKEN_TYPE_MINUS_OPERATOR
} dzen_token_type;

typedef enum dzen_value_modifier {
    DZEN_VALUE_MODIFIER_NONE,
    DZEN_VALUE_MODIFIER_INCREMENT,
    DZEN_VALUE_MODIFIER_DECREMNT
} dzen_value_modifier;

/**
* A token which represents
* a logical part like a function
* of the input
*/
struct dzen_token {
    dzen_token_type type;
    dzen_value_modifier value_modifier;
    char *string_value;
    int integer_value;
    struct dzen_token *parameter_list;
    struct dzen_token *next;
};

/**
* A struct representing a cache used to
* build a iterable list of token. The
* root_token is the main entry point
* to iterate through the whole token
* list.
*/
struct dzen_parser_cache {
    int is_initialized;
    struct dzen_token *previous_token;
    struct dzen_token *root_token;
    int token_count;
    int read_pos;
    char *input;
} dzen_parser_cache;

#define DZEN_NOT_SET -1

/************** token helper functions **************/

/**
* token.type = DZEN_TOKEN_TYPE_FUNCTION
* token.string_value = "function_name"
* token.parameter_list = [dzen_token, dzen_token, ...]
*/
struct dzen_token make_function_token(char *name, struct dzen_token parameter[]);

/**
* token.type = DZEN_TOKEN_TYPE_SIZE_PARAMETER
* token.parameter_list = [
*      width {DZEN_TOKEN_TYPE_NUMBER},
*      height {DZEN_TOKEN_TYPE_NUMBER}
* ]
*/
struct dzen_token make_size_parameter_token(struct dzen_token width, struct dzen_token height);

/**
* token.type = DZEN_TOKEN_TYPE_POSITIONING_PARAMETER
* token.value_modifier = DZEN_VALUE_MODIFIER_INCREMENT | DZEN_TOKEN_TYPE_MINUS_OPERATOR | DZEN_VALUE_MODIFIER_NONE
* token.integer_value = 0
*/
struct dzen_token make_positioning_parameter_token(int operator_type, struct dzen_token value);

/**
* token.type = DZEN_TOKEN_TYPE_POSITIONING_PARAMETER_LIST
* token.parameter_list = [
*      x {DZEN_TOKEN_TYPE_POSITIONING_PARAMETER},
*      y {DZEN_TOKEN_TYPE_POSITIONING_PARAMETER}
* ]
*/
struct dzen_token make_positioning_parameter_list_token(struct dzen_token x, struct dzen_token y);

/**
* token.type = DZEN_TOKEN_TYPE_NAME_IDENTIFIER
* token.string_value = "value"
*/
struct dzen_token make_name_identifier_token(char *name);

/**
* token.type = DZEN_TOKEN_TYPE_CONSTANT
* token.string_value = "_NAME"
*/
struct dzen_token make_constant_token(char *constant_name);

/**
* token.type = DZEN_TOKEN_TYPE_TEXT
* token.string_value = "Example text"
*/
struct dzen_token make_text_token(char *text);

/**
* token.type = DZEN_TOKEN_TYPE_FILE_PATH
* token.string_value = "../path/to/file.png"
*/
struct dzen_token make_file_path_token(char *path);

/**
* token.type = DZEN_TOKEN_TYPE_NUMBER
* token.integer_value = 0
*/
struct dzen_token make_number_token(int value);

/**
* token.type = DZEN_TOKEN_TYPE_COLOR_KEYWORD
* token.string_value = "red"
*/
struct dzen_token make_color_keyword_token(char *name);

/**
* token.type = DZEN_TOKEN_TYPE_HEX_COLOR
* token.string_value = "#FF00FF"
*/
struct dzen_token make_hex_color_token(char *hex);

/**
* token.type = DZEN_TOKEN_TYPE_PLUS_OPERATOR
*/
struct dzen_token make_plus_operator_token();

/**
* token.type = DZEN_TOKEN_TYPE_MINUS_OPERATOR
*/
struct dzen_token make_minus_operator_token();

/**
* Creates a new root token
*/
struct dzen_token *make_root_token();


/************** macros **************/

#define MAKE_DZEN_FUNCTION_TOKEN(name, parameter) make_function_token(name, parameter)
#define MAKE_DZEN_SIZE_PARAMETER_TOKEN(width, height) make_size_parameter_token(width, height)
#define MAKE_DZEN_POSITIONING_PARAMETER_TOKEN(operator_type, value) make_positioning_parameter_token(operator_type, value)
#define MAKE_DZEN_POSITIONING_PARAMETER_LIST_TOKEN(x, y) make_positioning_parameter_list_token(x, y)

#define MAKE_DZEN_COLOR_KEYWORD_TOKEN(name) make_color_keyword_token(name)
#define MAKE_DZEN_HEX_COLOR_TOKEN(hex) make_hex_color_token(hex)
#define MAKE_DZEN_NAME_IDENTIFIER_TOKEN(name) make_name_identifier_token(name)
#define MAKE_DZEN_NUMBER_TOKEN(value) make_number_token(value)
#define MAKE_DZEN_TEXT_TOKEN(text) make_text_token(text)
#define MAKE_DZEN_FILE_PATH_TOKEN(path) make_file_path_token(path)
#define MAKE_DZEN_CONSTANT_TOKEN(constant_name) make_constant_token(constant_name)

#define DZEN_ROOT_TOKEN make_root_token()
#define DZEN_MINUS_OPERATOR_TOKEN make_minus_operator_token()
#define DZEN_PLUS_OPERATOR_TOKEN make_plus_operator_token()


/************** parser helper functions **************/

typedef struct dzen_token *parser_token;

/**
* The default parser cache which should be
* initialized by using create_parser_cache()
*/
struct dzen_parser_cache parser_cache;

/**
* Initializes the parser cache, must be called before
* adding new token to the token list.
*/
void create_parser_cache();

int get_next_parser_input_char();

/**
* Helper function to add a token to a specific token list
*/
parser_token insert_next_token(parser_token list, struct dzen_token token);

/**
* Adds a new token to the token list
*/
void add_to_token_list(struct dzen_token token);

#endif