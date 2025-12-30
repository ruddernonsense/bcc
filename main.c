#include <stdio.h>

#include "tokens.h"
#include "utils.h"

#include "utf8.h"

int main(int argc, char **argv) {

  // String src_code = string_from_file("main.c");

  Tokenizer tokenizer = {0};
  tokenizer_create(&tokenizer, mk_string("for(int i = 0; i < 10; i++)"));

  printf("%s\n", tokenizer.content.str);
  Token token;
  do {
    token = tokenizer_next(&tokenizer);
    printf("%s : %s\n", token_type_to_str(token.token_type), token.raw.str);

  } while ((token.token_type != tk_error && token.token_type != tk_eof));

  tokenizer_free(&tokenizer);

  return 0;
}
