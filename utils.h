#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))

#define TODO(...)                                                              \
  {                                                                            \
    fprintf(stderr, "TODO: on line %d, in %s(), inside of %s", __LINE__,       \
            __func__, __FILE__);                                               \
    __VA_OPT__(fprintf(stderr, " - "); fprintf(stderr, __VA_ARGS__);)          \
    fprintf(stderr, "\n");                                                     \
    exit(EXIT_FAILURE);                                                        \
  }

typedef struct {
  size_t length;
  char *str;
} String;

typedef String StringSlice;

String mk_string(char *cstr);
void string_free(String str);
String copy_string(String str);
String string_slice(char *cstr, size_t length);

#define INFO(...)                                                              \
  {                                                                            \
    fprintf(stderr, "[INFO] on line %d, in %s(), inside of %s", __LINE__,      \
            __func__, __FILE__);                                               \
    __VA_OPT__(fprintf(stderr, " - "); fprintf(stderr, __VA_ARGS__);)          \
    fprintf(stderr, "\n");                                                     \
  }
