#include "main.c"
#include "base.c"

#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
  KryptonMain();
  return 0;
}

int PlatformWriteStdout(String str) {
  return write(1, str.value, str.length);
}
