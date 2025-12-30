
#include "tokens.h"
#include "utf8.h"
#include <ctype.h>

char *token_type_to_str(Token_type type) {
  switch (type) {
#define X(tt)                                                                  \
  case tk_##tt:                                                                \
    return #tt;
    TOKEN_TYPES
#undef X
  default:
    return "Unknown token type!";
  }
}

char *keyword_to_str(Keyword keyword) {
  switch (keyword) {
#define X(x)                                                                   \
  case kw_##x:                                                                 \
    return #x;
    KEYWORDS
#undef X
  default:
    return "Unknown Token Type!";
  }
}

static char *keywords[] = {
#define X(x) #x,
    KEYWORDS
#undef X
};

static size_t keywords_count =
#define X(x) +1
    KEYWORDS
#undef X
    ;

static char *punctuators[] = {
#define X(_, x) x,
    PUNCTUATORS
#undef X
};

static size_t punctuator_count =
#define X(x, y) +1
    PUNCTUATORS
#undef X
    ;

static char *encoding_prefixs[] = {
#define X(x) #x,
    ENCODINGPREFIXS
#undef X
};

static size_t encoding_prefixs_count =
#define X(_) +1
    ENCODINGPREFIXS
#undef X
    ;

#define TRY_TOKENIZE(tok)                                                      \
  token = tok(tokenizer);                                                      \
  if (token.token_type != tk_error) {                                          \
    tokenizer->offset += token.raw.length;                                     \
    tokenizer->column += token.raw.length;                                     \
    return token;                                                              \
  }

int tokenizer_create(Tokenizer *tokenizer, String input) {
  tokenizer->column = 0;
  tokenizer->line = 0;
  tokenizer->offset = 0;
  tokenizer->content = input;
  tokenizer->td_chain = NULL;
  return 0;
}
void tokenizer_free(Tokenizer *tokenizer) {
  string_free(tokenizer->content);
  for (Typedef_scope *tds = tokenizer->td_chain; tds != NULL; tds = tds->next) {
    TODO();
  }
  return;
}

Token tokenizer_peek(Tokenizer *tokenizer) {
  TODO();
  return (Token){};
}

// returns true if the str is at the current offset
bool tk_expect(Tokenizer *tokenizer, char *str) {
  if (tokenizer->offset + strlen(str) <= tokenizer->content.length) {
    return !strncmp(tokenizer->content.str + tokenizer->offset, str,
                    strlen(str));
  }
  return false;
}

Token tokenize_keyword(Tokenizer *tokenizer) {
  Token token = {0};
  for (size_t index = 0; index < keywords_count; index++) {
    char *kw = keywords[index];
    if (tk_expect(tokenizer, kw)) {
      token.token_type = tk_keyword;
      token.content.keyword = (Keyword)index;
      token.column = tokenizer->column;
      token.line = tokenizer->line;
      token.raw = mk_string(kw);
      return token;
    }
  }

  return token;
}

Token tokenize_punctuator(Tokenizer *tokenizer) {
  Token token = {0};
  for (size_t index = 0; index < keywords_count; index++) {
    char *pn = punctuators[index];
    if (tk_expect(tokenizer, pn)) {
      token.token_type = tk_punctuator;
      token.content.punctuator = (Punctuator)index;
      token.column = tokenizer->column;
      token.line = tokenizer->line;
      token.raw = mk_string(pn);
      return token;
    }
  }

  return token;
}

Token tokenize_strlit(Tokenizer *tokenizer);

Token tokenize_identifier(Tokenizer *tokenizer) {
  Token token = {0};

  size_t startb = tokenizer->offset;
  size_t length = 0;

  if (is_xid_start(tokenizer->content.str + tokenizer->offset + length)) {
    length +=
        codepoint_length(tokenizer->content.str + tokenizer->offset + length);
    while (
        is_xid_continue(tokenizer->content.str + tokenizer->offset + length)) {
      length +=
          codepoint_length(tokenizer->content.str + tokenizer->offset + length);
    }

    token.token_type = tk_identifier;
    token.column = tokenizer->column;
    token.line = tokenizer->line;
    token.raw.length = length;
    token.raw.str = malloc(token.raw.length * sizeof(char) + 1);
    strncpy(token.raw.str, tokenizer->content.str + startb, token.raw.length);
  }

  return token;
}

// //A.1.5 Constants
// //(6.4.4)
//
// (6.4.4.1)
//
//  //(6.4.4.1)
//  decimal-constant ::=
//    nonzero-digit
//    | decimal-constant "'"? digit
//
//  //(6.4.4.1)
//  octal-constant ::=
//    '0' octal-constant "'"? octal-digit
//  //(6.4.4.1)
//  hexadecimal-constant ::=
//    hexadecimal-prefix hexadecimal-digit-sequence
//
//  //(6.4.4.1)
//  binary-constant ::=
//    binary-prefix binary-digit
//    | binary-constant "'"? binary-digit
//
//  //(6.4.4.1)
//  hexadecimal-prefix ::= //one of
//    "0x" | "0X"
//
//  //(6.4.4.1)
//  binary-prefix ::= //one of
//    "0b" | "0B"
//
//  //(6.4.4.1)
//  nonzero-digit ::= //one of
//    [1-9]
//
//  //(6.4.4.1)
//  octal-digit ::= //one of
//    [0-7]
//
//  hexadecimal-digit-sequence ::=
//    hexadecimal-digit
//    | hexadecimal-digit-sequence "'"? hexadecimal-digit
//
//  //(6.4.4.1)
//  hexadecimal-digit ::= //one of
//    [A-Fa-f0-9]
//
//  //(6.4.4.1)
//  binary-digit ::= //one of
//    [01]
//
//  //(6.4.4.1)
//  integer-suffix ::=
//    unsigned-suffix long-suffix?
//    | unsigned-suffix long-long-suffix
//    | unsigned-suffix bit-precise-int-suffix
//    | long-suffix unsigned-suffix?
//    | long-long-suffix unsigned-suffix?
//    | bit-precise-int-suffix unsigned-suffix?
//
//  //(6.4.4.1)
//  bit-precise-int-suffix ::= //one of
//    "wb" | "WB"
//
//  //(6.4.4.1)
//  unsigned-suffix ::= //one of
//    'u' | 'U'
//
//  //(6.4.4.1)
//  long-suffix ::= //one of
//    'l' | 'L'
//
//  //(6.4.4.1)
//  long-long-suffix ::= //one of
//    "ll" | "LL"

//  integer-constant ::=
//    decimal-constant integer-suffix?
//    | octal-constant integer-suffix?
//    | hexadecimal-constant integer-suffix?
//    | binary-constant integer-suffix?

#warning Not standards compliant lmao
Token tokenize_integer_constant(Tokenizer *tokenizer) {
  Token token;

  size_t length = 0;
  while (isdigit(tk_peek(length))) {
    length++;
  }

  if (length > 0) {
    token.token_type = tk_constant;
    token.column = tokenizer->column;
    token.line = tokenizer->line;

    token.raw.length = length;
    token.raw.str = malloc(token.raw.length * sizeof(char) + 1);

    strncpy(token.raw.str, tokenizer->content.str + tokenizer->offset, length);

    token.content.constant.constant_type = ct_integer_constant;
    token.content.constant.value.integer = atol(token.raw.str);
  }

  return token;
}

Token tokenize_floating_constant(Tokenizer *tokenizer) { TODO(); }
Token tokenize_enumeration_constant(Tokenizer *tokenizer) { TODO(); }
Token tokenize_character_constant(Tokenizer *tokenizer) { TODO(); }
Token tokenize_predefined_constant(Tokenizer *tokenizer) { TODO(); }

// constant ::=
//   integer-constant
//   | floating-constant
//   | enumeration-constant
//   | character-constant
//   | predefined-constant
Token tokenize_constant(Tokenizer *tokenizer) {
  Token token;

  TRY_TOKENIZE(tokenize_integer_constant);
  TRY_TOKENIZE(tokenize_floating_constant);
  TRY_TOKENIZE(tokenize_enumeration_constant);
  TRY_TOKENIZE(tokenize_character_constant);
  TRY_TOKENIZE(tokenize_predefined_constant);

  return token;
}

void skip_whitespace(Tokenizer *tokenizer) {
  while (isspace(tk_peek(0))) {

    tokenizer->column++;
    if (tk_peek(0) == '\n') {
      tokenizer->column = 0;
      tokenizer->line++;
    }

    tokenizer->offset++;
  }
}

Token tokenize_line_comment(Tokenizer *tokenizer) {

  return (Token){}; // always fail lmao
}

Token tokenizer_next(Tokenizer *tokenizer) {
  Token token = {0};
  skip_whitespace(tokenizer);

  if (tokenizer->offset >= tokenizer->content.length) {
    token.column = tokenizer->column;
    token.line = tokenizer->line;
    token.token_type = tk_eof;
    return token;
  }

  // line comment?
  if (tk_expect(tokenizer, "//")) {
    while (tk_peek(0) != '\n') {

      tokenizer->offset++;
    }
  }

  TRY_TOKENIZE(tokenize_punctuator);
  TRY_TOKENIZE(tokenize_keyword);
  TRY_TOKENIZE(tokenize_identifier);
  TRY_TOKENIZE(tokenize_integer_constant);

  return token;
}

void tokenizer_push_typedef_scope(Tokenizer *tokenizer) {
  TODO();
  return;
}
void tokenizer_push_typedef_name(Tokenizer *tokenizer, String typedef_name) {
  TODO();
  return;
}
void tokenizer_pop_typedef_scope(Tokenizer *tokenizer) {
  TODO();
  return;
}
