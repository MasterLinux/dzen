#import "dzen_data_types.h"
#import "grammar.c"

struct dzen_token *dzen_parse(char *input) {
    create_parser_cache();
    parser_cache.input = input;

    while (yyparse());

    return parser_cache.root_token;
};

/**
* example function
*/
void dzen_token_iterate() {
    struct dzen_token *root = dzen_parse((char *) "^test(2x2)^lol()");

    cout << "token_count: " << parser_cache.token_count << endl;

    struct dzen_token *next = root->next;

    while (next != NULL) {
        cout << "token type: " << (*next).type << " - token name: " << (*next).string_value << endl;
        next = (*next).next;
    }
}