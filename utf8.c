#include "utf8.h"
#include "xid_tables.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// great site for this information: https://tonsky.me/blog/unicode/
uint32_t extract_codepoint(char *c) {
  uint8_t *chars = (uint8_t *)c;
  uint32_t codepoint;

  // yes, I know we're checking indexes without any guards. this surely will not
  // come back to bite us in the ass.
  if ((chars[0] & 0x80) == 0) { // one byte long?
    codepoint = chars[0];
  } else if ((chars[0] & 0xE0) == 0xC0) { // two bytes long?
    codepoint = (chars[0] & 0x1F) << 6 | (chars[1] & 0x3F);
  } else if ((chars[0] & 0xF0) == 0xE0) { // three bytes
    codepoint =
        (chars[0] & 0x0F) << 12 | (chars[1] & 0x3F) << 6 | (chars[2] & 0x3F);
  } else if ((chars[0] & 0xF8) == 0xF0) { // four bytes long?
    codepoint = (chars[0] & 0x0F) << 18 | (chars[1] & 0x3F) << 12 |
                (chars[2] & 0x3F) << 6 | (chars[3] & 0x3F);
  } else {
    // yes, I know this is bad, but for now it will suffice. (famous last words)
    fprintf(stderr, "invalid codepoint?\n");
    exit(EXIT_FAILURE);
  }
  return codepoint;
}

size_t codepoint_length(char *c) {
  uint8_t *chars = (uint8_t *)c;

  if ((chars[0] & 0x80) == 0) { // one byte long?
    return 1;
  } else if ((chars[0] & 0xE0) == 0xC0) { // two bytes long?
    return 2;
  } else if ((chars[0] & 0xF0) == 0xE0) { // three bytes
    return 3;
  } else if ((chars[0] & 0xF8) == 0xF0) { // four bytes long?
    return 4;
  } else {
    fprintf(stderr, "invalid codepoint?\n");
    exit(EXIT_FAILURE);
  }
}

bool search_range_table(char *c, range table[]) {

  uint32_t codepoint = extract_codepoint(c);

  // linear search for now lmao,  also scuffed ass bounds checking?
  for (size_t index = 0; table[index].low != 0x11FFFF; index++) {
    range r = table[index];
    if (r.low <= codepoint && codepoint <= r.high) {
      return true;
    }
  }

  return false;
}

bool is_xid_start(char *c) { return search_range_table(c, XID_START); }
bool is_xid_continue(char *c) { return search_range_table(c, XID_CONTINUE); }
