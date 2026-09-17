#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	int input;
	char *shell_path;
	uid_t uid;
	gid_t gid;

	input = atoi(argv[1]);

	if (input == 0x1a7) {
		shell_path = strdup("/bin/sh");
		gid = getegid();
		uid = geteuid();

		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);

		execv("/bin/sh", &shell_path);
	} else {
		fwrite("No !\n", 1, 5, stderr);
	}

	return 0;
}
