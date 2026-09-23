/*
 * Crackme #3 — XOR transform (algorithm reversal)
 *
 * RE aspects:  XOR transforms are super common and can be
 * hard to spot without practice. The string is not stored anywhere
 * and this one requires actual algorithm analysis
 * Password to type: "XorTangoBravo"
 * (stored bytes below are each plaintext char XOR'd with 0x5A;
 *  regenerate with the gen_xor.py helper if you change the password/key)
 */
#include <stdio.h>
#include <string.h>

static const unsigned char encoded[] = {0x02, 0x35, 0x28, 0x0E, 0x3B,
                                        0x34, 0x3D, 0x35, 0x18, 0x28,
                                        0x3B, 0x2C, 0x35};
static const unsigned char xor_key = 0x5A;

int main(void) {
  char input[64];

  printf("Enter password: ");
  scanf("%s", input);

  size_t len = sizeof(encoded);
  if (strlen(input) != len) {
    printf("Access denied.\n");
    return 0;
  }

  int ok = 1;
  for (size_t i = 0; i < len; i++) {
    if ((unsigned char)(input[i] ^ xor_key) != encoded[i]) {
      ok = 0;
      break;
    }
  }

  printf(ok ? "Access granted.\n" : "Access denied.\n");
  return 0;
}
