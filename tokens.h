#pragma once

#include <stddef.h>
#include <stdint.h>

#include "utils.h"

#define TOKEN_TYPES                                                            \
  X(error)                                                                     \
  X(eof)                                                                       \
  X(keyword)                                                                   \
  X(identifier)                                                                \
  X(constant)                                                                  \
  X(string_literal)                                                            \
  X(punctuator)                                                                \
  X(header_name)                                                               \
  X(pp_number)                                                                 \
  X(character_constant)

// this will probably get changed lmao
typedef enum {
#define X(kw) tk_##kw,
  TOKEN_TYPES
#undef X
  // this is fucking nonsense lmfao
  // "each universal-character-name that cannot be one of the above",
  // "each non-white-space character that cannot be one of the above",
} Token_type;

char *token_type_to_str(Token_type type);

#define TOKEN_FMT "Token<%s, (%zu, %zu), \"%s\">"
#define TOKEN_PRNT(t)                                                          \
  token_type_to_str((t).type), (t).line, (t).column, (t).value.str

#define KEYWORDS                                                               \
  X(alignas)                                                                   \
  X(alignof)                                                                   \
  X(_Atomic)                                                                   \
  X(auto)                                                                      \
  X(_BitInt)                                                                   \
  X(bool)                                                                      \
  X(break)                                                                     \
  X(case)                                                                      \
  X(char)                                                                      \
  X(_Complex)                                                                  \
  X(const)                                                                     \
  X(constexpr)                                                                 \
  X(continue)                                                                  \
  X(_Decimal128)                                                               \
  X(_Decimal32)                                                                \
  X(_Decimal64)                                                                \
  X(default)                                                                   \
  X(do)                                                                        \
  X(double)                                                                    \
  X(else)                                                                      \
  X(enum)                                                                      \
  X(extern)                                                                    \
  X(false)                                                                     \
  X(float)                                                                     \
  X(for)                                                                       \
  X(_Generic)                                                                  \
  X(goto)                                                                      \
  X(if)                                                                        \
  X(_Imaginary)                                                                \
  X(inline)                                                                    \
  X(int)                                                                       \
  X(long)                                                                      \
  X(_Noreturn)                                                                 \
  X(nullptr)                                                                   \
  X(register)                                                                  \
  X(restrict)                                                                  \
  X(return)                                                                    \
  X(short)                                                                     \
  X(signed)                                                                    \
  X(sizeof)                                                                    \
  X(static)                                                                    \
  X(static_assert)                                                             \
  X(struct)                                                                    \
  X(switch)                                                                    \
  X(thread_local)                                                              \
  X(true)                                                                      \
  X(typedef)                                                                   \
  X(typeof)                                                                    \
  X(typeof_unqual)                                                             \
  X(union)                                                                     \
  X(unsigned)                                                                  \
  X(void)                                                                      \
  X(volatile)                                                                  \
  X(while)

typedef enum {
#define X(x) kw_##x,
  KEYWORDS
#undef X
} Keyword;

char *keyword_to_str(Keyword keyword);

#define PUNCTUATORS                                                            \
  X(concat_alt, "%:%:")                                                        \
  X(ellipsis, "...")                                                           \
  X(left_shift_assign, "<<=")                                                  \
  X(right_shift_assign, ">>=")                                                 \
  X(arrow, "->")                                                               \
  X(increment, "++")                                                           \
  X(decrement, "--")                                                           \
  X(left_shift, "<<")                                                          \
  X(right_shift, ">>")                                                         \
  X(less_equal, "<=")                                                          \
  X(greater_equal, ">=")                                                       \
  X(equal, "==")                                                               \
  X(not_equal, "!=")                                                           \
  X(logical_and, "&&")                                                         \
  X(logical_or, "||")                                                          \
  X(scope, "::")                                                               \
  X(mul_assign, "*=")                                                          \
  X(div_assign, "/=")                                                          \
  X(mod_assign, "%=")                                                          \
  X(add_assign, "+=")                                                          \
  X(sub_assign, "-=")                                                          \
  X(and_assign, "&=")                                                          \
  X(xor_assign, "^=")                                                          \
  X(or_assign, "|=")                                                           \
  X(concat, "##")                                                              \
  X(left_bracket_alt, "<:")                                                    \
  X(right_bracket_alt, ":>")                                                   \
  X(left_brace_alt, "<%")                                                      \
  X(right_brace_alt, "%>")                                                     \
  X(hash_alt, "%:")                                                            \
  X(left_bracket, "[")                                                         \
  X(right_bracket, "]")                                                        \
  X(left_paren, "(")                                                           \
  X(right_paren, ")")                                                          \
  X(left_brace, "{")                                                           \
  X(right_brace, "}")                                                          \
  X(dot, ".")                                                                  \
  X(ampersand, "&")                                                            \
  X(asterisk, "*")                                                             \
  X(plus, "+")                                                                 \
  X(minus, "-")                                                                \
  X(tilde, "~")                                                                \
  X(exclamation, "!")                                                          \
  X(slash, "/")                                                                \
  X(percent, "%")                                                              \
  X(less_than, "<")                                                            \
  X(greater_than, ">")                                                         \
  X(caret, "^")                                                                \
  X(pipe, "|")                                                                 \
  X(question, "?")                                                             \
  X(colon, ":")                                                                \
  X(semicolon, ";")                                                            \
  X(assign, "=")                                                               \
  X(comma, ",")                                                                \
  X(hash, "#")

typedef enum {
#define X(x, _) pn_##x,
  PUNCTUATORS
#undef X
} Punctuator;

#define ENCODINGPREFIXS                                                        \
  X(u8)                                                                        \
  X(u)                                                                         \
  X(U)                                                                         \
  X(L)

typedef enum {
#define X(x) ep_##x,
  ENCODINGPREFIXS
#undef X
} EncodingPrefix;

#define CONSTANTTYPES                                                          \
  X(integer_constant)                                                          \
  X(floating_constant)                                                         \
  X(enumeration_constant)                                                      \
  X(character_constant)                                                        \
  X(predefined_constant)

typedef enum {
#define X(x) ct_##x,
  CONSTANTTYPES
#undef X
} ConstantType;

typedef struct {
  size_t line, column;
  Token_type token_type;
  union {
    Punctuator punctuator;
    Keyword keyword;
    struct {
      EncodingPrefix encoding_prefix;
      String real_str;
    } string_literal;
    struct {
      ConstantType constant_type;
      union {
        int64_t integer;
      } value;
    } constant;
  } content;
  String raw; // string view into raw source code
} Token;

// each scope is a dynamic array of strings pointing to typedef names?
typedef struct Typedef_scope {
  struct Typedef_scope *next;
} Typedef_scope;

typedef struct {
  String content; // takes ownership
  size_t line, column, offset;
  Typedef_scope *td_chain;
} Tokenizer;

int tokenizer_create(Tokenizer *tokenizer, String input);
void tokenizer_free(Tokenizer *tokenizer);

Token tokenizer_peek(Tokenizer *tokenizer);
Token tokenizer_next(Tokenizer *tokenizer);

void tokenizer_push_typedef_scope(Tokenizer *tokenizer);
void tokenizer_push_typedef_name(Tokenizer *tokenizer, String typedef_name);
void tokenizer_pop_typedef_scope(Tokenizer *tokenizer);

#define tk_peek(n)                                                             \
  ((tokenizer->offset + (n) < tokenizer->content.length)                       \
       ? tokenizer->content.str[tokenizer->offset + (n)]                       \
       : 0)
