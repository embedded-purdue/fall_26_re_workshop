/*
 * Crackme #2 — Char-by-char comparison, reversed
 *
 * RE aspects: no single grep-able string match; must read the loop,
 * track the index variable, and notice it's indexing target[] backwards
 * (target[len - 1 - i]) against input[i].
 *
 * Password to type: the reverse of the stored target string.
 * target  = "ReverseReverse"
 * answer  = "esreveResreveR"
 */
#include <stdio.h>
#include <string.h>

static const char target[] = "ReverseReverse";

int main(void) {
  char input[64];

  printf("Enter password: ");
  scanf("%s", input);

  size_t len = strlen(target);
  if (strlen(input) != len) {
    printf("Access denied.\n");
    return 0;
  }

  int ok = 1;
  for (size_t i = 0; i < len; i++) {
    if (input[i] != target[len - 1 - i]) {
      ok = 0;
      break;
    }
  }

  printf(ok ? "Access granted.\n" : "Access denied.\n");
  return 0;
}
