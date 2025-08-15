#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *auth = NULL;
char *service = NULL;

int main(void) {
	char input[128];
	char temp[2];		// Local variable used in "auth" copy logic
	char service_buf[125];

	while (1) {
		printf("%p, %p\n", auth, service);

		// Read a line of input
		if (!fgets(input, sizeof(input), stdin))
			return 0;

		if (strncmp(input, "auth", 4) == 0) {
			auth = malloc(4);
			if (!auth) {
				perror("malloc");
				exit(1);
			}
			memset(auth, 0, 4);

			size_t len = strlen(temp);
			if (len < 31) {
				strcpy(auth, temp);
			}
		}

		else if (strncmp(input, "reset", 5) == 0) {
			free(auth);
			auth = NULL;
		}

		else if (strncmp(input, "service", 7) == 0) {
			service = strdup(service_buf);
		}

		else if (strncmp(input, "login", 5) == 0) {
			if (*(int *)(auth + 32) == 0) {
				fwrite("Password:\n", 1, 10, stdout);
			} else {
				system("/bin/sh");
			}
		}
	}
}
