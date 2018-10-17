#include "parse.h"
#include <sys/types.h>
#include <sys/wait.h>

int redirectRead(char* infile)
{
	int fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		perror("INPUT REDIRECTION FAILED");
	}
	return fd;
}

int redirectWrite(char append, char* outfile)
{
	int fd;
	if (append == 1)
	{
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	}
	else
	{
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, S_IRUSR | S_IWUSR);
	}
	if (fd < 0)
	{
		perror("OUTPUT REDIRECTION FAILED");
	}
	return fd;
}

int pipeProcess(int fdIn, int fdOut, char** argv, char* infile, char* outfile, char append)
{
    pid_t pid = fork();
	int newFd;

    if (pid < 0)
    {
        perror("Fork Failure");
        exit(1);
    }

    if (pid == 0)
    {
		if (fdIn == 0)
		{
			if (infile != NULL)
			{
				//printf("infile is not empty\n");
				newFd = redirectRead(infile);
				close(0);
				dup(newFd);
			}
		}

		if (fdOut == 1)
		{
			if (outfile != NULL)
			{
				//printf("outfile is not empty\n");
				newFd = redirectWrite(append, outfile);
				close(1);
				dup(newFd);
			}
		}

        if (fdIn != 0)
        {
			close(0);
            dup(fdIn);
        }

        if (fdOut != 1)
        {
			close(1);
            dup(fdOut);
        }

		//fprintf(stderr, "fdIn: %d, fdOut: %d\n", fdIn, fdOut);
        if (execvp(argv[0], argv) < 0)
		{
			perror("EXEC ERROR:");
			return -1;
		}
    }

	if (pid > 0)
	{
		wait(NULL);
	}

    return pid;

}

int runCmd(int numPipes, struct commandLine* cmdLine)
{
    int i;
    pid_t pid;
    int fdIn = 0;
    int fd[2];
    int nextCmd = 0;

    char** temp = cmdLine->argv;

	char* infile = cmdLine->infile;
	char* outfile = cmdLine->outfile;
	char append = cmdLine->append;

	//printf("outfile: %s\n", outfile);
	//printf("infile: %s\n", infile);

    for (i = 0; i < numPipes; i++)
    {
        pipe(fd);

        if (nextCmd != 0)
        {
            temp = &(cmdLine->argv[cmdLine->cmdStart[i]]);
        }

        pipeProcess(fdIn, fd[1], temp, infile, outfile, append);
		if (fdIn != 0)
		{
			close(fdIn);
		}
		close(fd[1]);

        fdIn = fd[0];

        nextCmd = 1;
    }
	

	temp = &(cmdLine->argv[cmdLine->cmdStart[i]]);

	pipeProcess(fdIn, 1, temp, infile, outfile, append);
	if (fdIn != 0)
	{
		close(fdIn);
	}

	//wait
	wait(NULL);

    return 1;
}

int main(int argc, char *argv[])
{
	FILE *input;

	if(argc == 2)
    {
		input = fopen(argv[1], "r");
		if(input == NULL)
		{
			perror(argv[1]);
			exit(1);
		}

		//printf("%s\n", argv[1]);
    }
    else
    {
	assert(argc == 1);
	input = stdin;

	printf("? ");
	fflush(stdout);
    }

	char line[MAX_LINE];

    setlinebuf(input);
    while(fgets(line, sizeof(line), input))
    {
		int i;
		struct commandLine cmdLine;

		if(line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = '\0';   /* zap the newline */

		//printf("line: %s\n", line);
		Parse(line, &cmdLine);

		if (cmdLine.numCommands == 0)
		{
			//printf("num commands is 0.\n");
			continue;
		}

		if (strcmp(cmdLine.argv[0], "cd") == 0)
		{
			//printf("cd here:\n");
			if (chdir(cmdLine.argv[1]) < 0)
			{
				perror("CD FAILED");
				exit(1);
			}
		}
		else
		{
			runCmd(cmdLine.numCommands - 1, &cmdLine);
		}
		//execvp(cmdLine.argv[0], &(cmdLine.argv[0]));

		if(input == stdin)
		{
			printf("? ");
			fflush(stdout);
		}
    }

    return 0;
}

