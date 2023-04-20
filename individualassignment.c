/*NURUL FARZANA BINTI SUFIAN*/

#include <stdlib.h> /*needed to define exit()*/
#include <unistd.h> /*needed for fork()*/
#include <stdio.h> /*needed for printf()*/
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define QTY_LENGTH 4
#define QTY_WAIT_INTERVAL 2

void getQTY (char qty[QTY_LENGTH + 1])
{
	srand(getpid() + getppid());

	qty[0] = 49 + rand() % 7;

	for(int i = 1; i < QTY_LENGTH; i++)
	{
		qty[i] = 48 + rand() % 7;
	}
	qty[QTY_LENGTH] = '\0';
}
void sigint_handler(int sig);
char s[200];
int main()
{
	int pipefds[2];
	char qty[QTY_LENGTH + 1];
	char buffer[QTY_LENGTH + 1];
	char string[200];

	if(signal(SIGINT, sigint_handler) == SIG_ERR){
	perror("signal");
	exit(1);
	}

	pipe(pipefds);

	for (int i = 1; i < 3; i++){
	pid_t pid = fork();
	if(pid == 0){
	getQTY(qty); //generate quantity
	close(pipefds[0]); //close read fd
	write(pipefds[1], qty, QTY_LENGTH + 1);  //write QTY to pipe

	printf("Enter the fabric type: ");
	fgets(string, 200, stdin);
	printf("This is your fabric type: %s", string);
	printf("Generating QTY in child and sending to parent...\n");

	sleep(QTY_WAIT_INTERVAL); //delaying QTY generation intentionally

	exit(EXIT_SUCCESS);
	}

	if(pid > 0){
	wait(NULL); //waiting for child to finish

	close(pipefds[1]); //close write fd
	read(pipefds[0], buffer, QTY_LENGTH + 1); //read QTY from pipe
	close(pipefds[0]); //close read fd
	printf("Parent received QTY '%s' from child.\n\n", buffer);
	}
	}
	fgets(s, 200, stdin);
	return EXIT_SUCCESS;
}
void sigint_handler(int sig)
{
	printf("The process has been interrupted.\n");
}
