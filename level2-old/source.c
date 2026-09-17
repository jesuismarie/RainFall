#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void p(void) {
	char buffer[76];
	unsigned int ret_addr;

	fflush(stdout);

	gets(buffer);

	if ((ret_addr & 0xB0000000) == 0xB0000000) {
		printf("Blocked address: %p\n", (void*)ret_addr);
		exit(1);
	}

	puts(buffer);
	strdup(buffer);
}

int main() {
	p();
	return;
}