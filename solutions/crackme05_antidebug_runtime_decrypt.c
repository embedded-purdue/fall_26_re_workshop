/*
 * Crackme #5 — Anti-debugging + runtime-only decryption
 *
 * RE aspects: the clearest possible motivation for dynamic analysis.
 * The password is never present in plaintext anywhere in the binary —
 * it's XOR-decoded into a stack buffer only at runtime. A naive
 * PTRACE_TRACEME self-attach check also causes the program to bail out
 * if a debugger is already attached, so participants need to either
 * patch/NOP the check or set a breakpoint *after* it fires and step
 * from there.
 *
 * Password to type: "RuntimeOnly42"
 * (encoded bytes below are each plaintext char XOR'd with 0x77;
 *  regenerate with the gen_xor.py helper if you change the password/key)
 *
 * Build for Linux (uses <sys/ptrace.h>).
 */
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <unistd.h>

static void anti_debug_check(void) {
  if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
    /* A debugger is already attached — ptrace(PTRACE_TRACEME) fails */
    printf("Access denied.\n");
    _exit(1);
  }
}

static void decrypt_password(char *buf, const unsigned char *enc, size_t len,
                             unsigned char key) {
  for (size_t i = 0; i < len; i++) {
    buf[i] = (char)(enc[i] ^ key);
  }
}

int main(void) {
  anti_debug_check();

  static const unsigned char encoded_pw[] = {0x25, 0x02, 0x19, 0x03, 0x1E,
                                             0x1A, 0x12, 0x38, 0x19, 0x1B,
                                             0x0E, 0x43, 0x45};
  static const unsigned char key = 0x77;

  char password[sizeof(encoded_pw) + 1] = {0};
  decrypt_password(password, encoded_pw, sizeof(encoded_pw), key);

  char input[64];
  printf("Enter password: ");
  scanf("%s", input);

  if (strcmp(input, password) == 0) {
    printf("Access granted.\n");
  } else {
    printf("Access denied.\n");
  }
  return 0;
}
