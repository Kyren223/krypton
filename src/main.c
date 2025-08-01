#include "base.c"
#include "platform.h"

int KryptonMain() {
  // char msg[3] = { 'h', 'e', 'y' };
  if (true) {
    PlatformWriteStdout(S("Hey"));
  }
  return 0;
}
