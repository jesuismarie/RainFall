# Level 0 Walkthrough

1. **List the contents of the home directory**

	```bash
	ls -l
	```

	Output shows the `level0` binary is SUID and owned by `level1`.

2. **Test the binary**

	```bash
	./level0
	```

	Result: segmentation fault when no arguments are passed.

3. **Copy the binary to local machine for analysis**

	```bash
	scp -P 4242 level0@<vm-ip>:/home/user/level0/level0 .
	```

4. **Open the binary in Ghidra**
	Analyze the main function. A key line is:

	```c
	int input = atoi(argv[1]);
	if (input == 0x1a7) {
		char *cmd = strdup("/bin/sh");
		setresgid(getegid(), getegid(), getegid());
		setresuid(geteuid(), geteuid(), geteuid());
		execv("/bin/sh", &cmd);
	}
	```

	It checks if the argument passed is equal to `0x1a7` (decimal: 423).

5. **Execute the binary with the correct argument**

	```bash
	./level0 423
	```

	This gives a shell with `level1` user privileges.

6. **Read the password for level1**

	```bash
	cat /home/user/level1/.pass
	```

	Output:

	```
	1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
	```
