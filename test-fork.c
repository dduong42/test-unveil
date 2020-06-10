#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void
test_permissions(void)
{
	FILE *fp = NULL;

	if ((fp = fopen("/etc/installurl", "r")) == NULL) {
		err(EXIT_FAILURE, "fopen /etc/installurl");
	}
	puts("[*] Was able to open /etc/installurl");
	fclose(fp);
	if ((fp = fopen("/etc/hosts", "r")) == NULL) {
		puts("[*] Wasn't able to open /etc/hosts");
	} else {
		puts("ERR: Was able to open /etc/hosts");
		fclose(fp);
	}
}

int
main(void)
{
	pid_t pid;
	int status;

	
	if (unveil("/bin/cat", "x") == -1) {
		err(EXIT_FAILURE, "unveil");
	}
	if (unveil("/etc/installurl", "r") == -1) {
		err(EXIT_FAILURE, "unveil");
	}
	if (unveil(NULL, NULL) == -1) {
		err(EXIT_FAILURE, "unveil");
	}
	puts("[*] Unveil called");
	test_permissions();
	puts("[*] Forking");
	pid = fork();
	switch (pid) {
	case -1:
		err(EXIT_FAILURE, "fork");
		/* no break, the program exits */
	case 0:
		test_permissions();
		puts("[*] Try permissions with exec");
		if (execlp("cat", "cat", "/etc/hosts", NULL) == -1) {
			err(EXIT_FAILURE, "execlp");
		}
		/* break isn't needed because execlp replaces the process */
	default:
		if (wait(&status) == -1) {
			err(EXIT_FAILURE, "wait");
		}
		if (WIFEXITED(status)) {
			puts("[*] Child terminated normally");
			printf("Exit status: %d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			puts("[*] Child terminated because of a signal");
			printf("Signal number: %d\n", WTERMSIG(status));
		}
	}
	return EXIT_SUCCESS;
}
