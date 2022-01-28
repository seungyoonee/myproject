#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

static void die(const char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char **argv)
{
	// declare necessary variables - buf, pid
	char buf[100];
	pid_t pid;

	// print the first prompt
	printf("port number: ");

	while((fgets(buf, sizeof(buf), stdin) != NULL))
	{
		if(buf[0] != '\n')
		{
			// replace newline with '\0'
			if(buf[strlen(buf) - 1] == '\n')
				buf[strlen(buf) - 1] = '\0';
	
			pid = fork();

			// die if fork() produced an error
			if(pid < 0)
			{
				die("fork failed");
			}
			// child process
			else if(pid == 0)
			{
				fprintf(stderr, "[pid=%d] ", (int)getpid());
				fprintf(stderr, "mdb-lookup-server started on port %s\n", buf);
				execl("./mdb-lookup-server-nc.sh", "mdb-lookup-server-nc.sh",
						buf, (char *)0);
				die("execl failed");
			}
		}

		// parent process
		while((pid = waitpid((pid_t) - 1, NULL, WNOHANG)) > 0)
		{
			fprintf(stderr, "[pid=%d] ", (int)pid);
			fprintf(stderr, "mdb-lookup-server terminated\n");
		}

		printf("port number: ");
	}

	// check if fgets() produced an error
	if(ferror(stdin))
		die("stdin error");

	return 0;
}

