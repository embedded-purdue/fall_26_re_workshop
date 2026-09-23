/*
 * Crackme #1 — Direct string comparison (baseline)
 *
 * RE aspects: Basic Ghidra work, finding strings in binaries
 *
 * Password: Ghidra4Life
 */
#include <stdio.h>
#include <string.h>

int main(void) {
  char input[64];
  const char *password = "Ghidra4Life";

  printf("Enter password: ");
  scanf("%s", input);

  if (strcmp(input, password) == 0) {
    printf("Access granted.\n");
  } else {
    printf("Access denied.\n");
  }
  return 0;
}
