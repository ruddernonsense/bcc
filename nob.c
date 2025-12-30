
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  Nob_Cmd cmd = {0};

  nob_cc(&cmd);
  nob_cc_flags(&cmd);
  nob_cmd_append(&cmd, "-ggdb");
  nob_cc_output(&cmd, "bcc");
  nob_cc_inputs(&cmd, "main.c");
  nob_cc_inputs(&cmd, "tokens.c");
  nob_cc_inputs(&cmd, "utils.c");
  nob_cc_inputs(&cmd, "utf8.c");

  if (!nob_cmd_run(&cmd))
    return 1;

  return 0;
}
