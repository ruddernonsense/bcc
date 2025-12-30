#pragma once

#include <stdbool.h>
#include <stddef.h>

// heavily "inspired" by https://github.com/unicode-rs/unicode-xid/

bool is_xid_start(char *c);
bool is_xid_continue(char *c);
size_t codepoint_length(char *c);
