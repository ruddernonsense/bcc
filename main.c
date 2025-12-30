#include <stdio.h>

#include "tokens.h"
#include "utils.h"

#include "utf8.h"

String string_from_file(char *str) {
  String string;

  FILE *fp = fopen(str, "rb");

  if (!fp) {
    fprintf(stderr, "Failed to open file lmao\n");
    exit(EXIT_FAILURE);
  }

  size_t length = 0;

  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  string.str = malloc(length + 1);
  fread(string.str, 1, length, fp);

  string.length = length;

  fclose(fp);

  return string;
}

int main(int argc, char **argv) {

  String src_code = string_from_file("main.c");
  // String src_code = mk_string("#");

  Tokenizer tokenizer = {0};
  tokenizer_create(&tokenizer, src_code);

  // printf("%s\n", tokenizer.content.str);
  Token token;
  do {
    token = tokenizer_next(&tokenizer);
    printf("%s : %s\n", token_type_to_str(token.token_type), token.raw.str);

  } while ((token.token_type != tk_error && token.token_type != tk_eof));

  tokenizer_free(&tokenizer);

  return 0;
}
