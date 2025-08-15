#include <stdio.h>
#include <stdlib.h>

int m; // Uninitialzed global variable in bss (set to 0)

void v(void)
{
	char input_buffer[520];

	fgets(input_buffer, 512, stdin);

	printf(input_buffer);

	// If m equals 64, trigger shell
	if (m == 64) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
}

int main(void) {
	v();
	return 0;
}
