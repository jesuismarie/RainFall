#include <stdio.h>
#include <stdlib.h>

int m = 0;  // global variable initialized to 0

void p(char *input) {
	printf(input);
}

void n(void) {
	char buffer[520];

	fgets(buffer, sizeof(buffer), stdin);

	p(buffer);

	if (m == 0x1025544) {
		system("/bin/cat /home/user/level5/.pass");
	}
}

int main(void) {
	n();
	return 0;
}
