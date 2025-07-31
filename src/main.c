#include "base.c"
#include "platform.h"

int KryptonMain() {
  // char msg[3] = { 'h', 'e', 'y' };
  PlatformWriteStdout(S("Hey"));
  return 0;
}
