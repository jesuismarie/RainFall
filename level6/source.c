#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void n(void) {
	system("/bin/cat /home/user/level7/.pass");
}

static void m(void *p1, int p2, char *p3, int p4, int p5) {
	(void)p1; (void)p2; (void)p3; (void)p4; (void)p5;
	puts("Nope");
}

typedef void (*handler_t)(void*, int, char*, int, int);

int main(int argc, char *argv[])
{
	char *str = malloc(64);
	uint32_t *addr = malloc(4);

	*addr = m;
	strcpy(str, argv[1]);

	((uint32_t (*)())*addr)();
}