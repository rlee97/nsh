/* parse.h: Header file for parse.c, which defines the datatype
** for a command line.
*/

#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 10240  /* longest accepted command line */
#define MAX_COMMANDS 32 /* Max # of commands (eg, "who | wc" is 2 commands) */
#define MAX_ARGS 32     /* maximum number of arguments per commad */


struct commandLine
{
    /*
    ** The line you give is copied into buf[].   If you were to implement
    ** back-quotes, for example, then you would have to change Parse() so
    ** that when it finds something in back-quotes, it copies the stuff
    ** between back-quotes into a new string (obeying rules about other
    ** quotes and '\' characters, if you implement them), then calls
    ** itself recursively, then puts the output back onto the command
    ** line.
    **    All the argv[] pointers point into this buffer, as do infile
    ** and outfile.
    */
    char buf[MAX_LINE];

    /*
    ** The argv array is a list of pointers to arguments.  The list ends
    ** with a NULL pointer, and then the next command follows.
    ** cmdStart[] is an integer index into the argv[] array, pointing
    ** at the start of a command.  The last command is marked by an
    ** extra NULL pointer, indicating "no more arguments".  So, for
    ** example, "who | wc -l >foo" becomes:
    **
    **  infile -> NULL      (-> means "is a pointer to")
    **  outfile -> "foo"
    **  append = 0;    (ie., append is false; 1 would be "append=true")
    **
    **  numCommands = 2
    **  cmdStart[0] = 0     (because "who" is in argv[0])
    **  cmdStart[1] = 2     (because "wc" is in argv[2])
    **  argv[0] -> "who"
    **  argv[1] -> NULL
    **  argv[2] -> "wc"
    **  argv[3] -> "-l"
    **  argv[4] -> NULL
    **  argv[5] -> NULL     (end-of-arguments flag)
    **
    ** The argv[] array is designed so that it can be passed to
    ** execvp() verbatim.  In the above example, there would be two
    ** calls to execvp:
    ** execvp(argv[0], &(argv[0])), and execvp(argv[2], &(argv[2])).
    ** Of course the two processes would have to be connected by a pipe
    ** first.
    */

    int numCommands;
    int cmdStart[MAX_COMMANDS];
    char *argv[(MAX_ARGS+1) * MAX_COMMANDS + 1];    /* +1 to hold the NULLs */
    char *infile, *outfile;
    char append;	/* Boolean: 1 means ">>", 0 means ">"

    /*
    ** Here would be a good place to add stuff like a boolean signifying
    ** background execution, for example.
    */
};

/*
** Function prototype for the scanner/parser
*/
int Parse(char *line, struct commandLine *sc);

extern void perror(const char *);
