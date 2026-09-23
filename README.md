# Reverse Engineering Workshop

Practice binaries and original source for the workshop.

## 1. Install Ghidra

Download the latest release from https://github.com/NationalSecurityAgency/ghidra/releases and follow the [installation steps](https://github.com/NationalSecurityAgency/ghidra). Ghidra needs a JDK (21+) installed.

## 2. Crack the binaries

1. In Ghidra, create a new project (**File → New Project**).
2. Import each file in `binaries/` (**File → Import File**), then double-click it and let Ghidra auto-analyze it.
3. Dig through the decompiled code and try to figure out the password for each one.

No peeking at the source until you've given it a shot!

## 3. Presentation

The workshop slides are in `resources/`.

## 4. Compiling exercise 5

Exercise 5 is meant to be solved with `gdb`, so compile it yourself on your own machine (note that this requires Linux or WSL because it uses ptrace.h):

```sh
# install tools (Debian/Ubuntu)
sudo apt install gcc gdb

# compile
gcc -o ex5 crackme05_antidebug_runtime_decrypt.c

# run it under gdb
gdb ./ex5
```

## 5. Running exercise 5 in gdb

You will notice that if you just run gdb with the above command and type `run`, the program will exit and say password failed.
This is because there is a built-in anti-debug checker. For the sake of saving time, I will tell you that this function is called anti_debug_check. You can confirm this in Ghidra.

In order to get by this debug checker, you will need to set a breakpoint right before the anti_debug_check checks if there is a debugger present. To do this, you will want to run `disassemble anti_debug_check` in gdb. This should return something similar to the following:

```
Dump of assembler code for function anti_debug_check:
   0x0000555555555199 <+0>:     push   %rbp
   0x000055555555519a <+1>:     mov    %rsp,%rbp
   0x000055555555519d <+4>:     mov    $0x0,%ecx
   0x00005555555551a2 <+9>:     mov    $0x1,%edx
   0x00005555555551a7 <+14>:    mov    $0x0,%esi
   0x00005555555551ac <+19>:    mov    $0x0,%edi
   0x00005555555551b1 <+24>:    mov    $0x0,%eax
   0x00005555555551b6 <+29>:    call   0x555555555090 <ptrace@plt>
   0x00005555555551bb <+34>:    test   %rax,%rax
   0x00005555555551be <+37>:    jns    0x5555555551d9 <anti_debug_check+64>
   0x00005555555551c0 <+39>:    lea    0xe41(%rip),%rax        # 0x555555556008
   0x00005555555551c7 <+46>:    mov    %rax,%rdi
   0x00005555555551ca <+49>:    call   0x555555555040 <puts@plt>
   0x00005555555551cf <+54>:    mov    $0x1,%edi
   0x00005555555551d4 <+59>:    call   0x555555555030 <_exit@plt>
   0x00005555555551d9 <+64>:    nop
   0x00005555555551da <+65>:    pop    %rbp
   0x00005555555551db <+66>:    ret
End of assembler dump.
```

You will notice that there is a call to ptrace@plt at a specific memory address, and that the return value is tested in the following instruction (+34 in the above case). Normally, when a debugger is present, -1 is returned from ptrace@plt. In order to change this, you will need to set a breakpoint immediately after the function call (memory address of the test line in my case). From there, you can check the return value of ptrace with `print $rax` (in my case, since that is the variable being testing on x86). You should see that it shows -1. In order to override this, you must then write `set $rax = 0` and `continue` should work and bring you to a password prompt.



From this point on, you should try to reverse engineer the password with the tools you have already learned! I'll give you a hint: `disassemble` is useful for another function in this program, too. If you can find that function, you may be able to piece together what it is doing.



Worth noting that in this case, you can always just toss the executable into Ghidra for a more full picture. In certain real scenarios, though, the firmware may be too complicated to analyze statically, or it may just be easier to see values at runtime (i.e. if things are allocated on the heap). These are perfect use-cases to runtime analysis.


