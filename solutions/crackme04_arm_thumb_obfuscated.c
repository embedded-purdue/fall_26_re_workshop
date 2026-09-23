/*
 * Crackme #4 — Obfuscated control flow + ARM/Thumb interworking
 *
 * RE aspects: Function Graph view to separate real paths from decoys,
 * opaque predicates, and the ARM/Thumb interworking gotcha — decoy_check
 * is compiled as ARM, real_check as Thumb, so the same binary contains
 * both instruction sets and Ghidra must correctly detect the mode switch
 * at the BX/BLX boundary or the decompiled output will look wrong.
 *
 * Password to type: "ThumbModeSecret"
 * (the decoy_check path is intentionally unreachable — its opaque
 *  predicate (x * 13) % 5 == 0 is always false for x = 7)
 */
#include <stdio.h>
#include <string.h>

__attribute__((noinline, target("arm"))) static int decoy_check(const char *s) {
  volatile int x = 7;
  if ((x * 13) % 5 == 0) { /* opaque predicate, always false */
    return strcmp(s, "AnotherRandomSecret") == 0;
  }
  return 0;
}

__attribute__((noinline, target("thumb"))) static int
real_check(const char *s) {
  const char *password = "ThumbModeSecret";
  return strcmp(s, password) == 0;
}

int main(void) {
  char input[64];

  printf("Enter password: ");
  scanf("%s", input);

  volatile int junk = (int)strlen(input) * 31 + 7;

  if ((junk & 1) == 0 && decoy_check(input)) {
    printf("Access granted.\n");
  } else if (real_check(input)) {
    printf("Access granted.\n");
  } else {
    printf("Access denied.\n");
  }
  return 0;
}
