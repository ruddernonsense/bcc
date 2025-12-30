#include "utils.h"

String mk_string(char *cstr) {
  String new_string = {0};
  new_string.length = strlen(cstr);
  new_string.str = (char *)malloc(new_string.length * sizeof(char) + 1);
  strcpy(new_string.str, cstr);

  return new_string;
}

void string_free(String str) {
  if (str.str != NULL)
    free(str.str);
}

String copy_string(String str) { return mk_string(str.str); }

String string_slice(char *cstr, size_t length) {
  String new_string = {0};
  new_string.length = MIN(length, strlen(cstr));
  new_string.str = cstr;

  return new_string;
}
