#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    int retVal = system(cmd);
    if (retVal != 0) // To cover the case of a -1 returned, otherwise I'd use `bool` for retVal
    {
        retVal = 1;
    }

    return !retVal;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];

    pid_t pid;
    int status;
    bool retVal;

    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    // Non-obvious from instructions, but accept only absolute paths.
    if (!strchr(command[0], '/'))
    {
        retVal = false;
    }
    else
    {
        // Fork a new process
        pid = fork();

        if (pid == -1)
        {
            perror("fork() failed: ");
            retVal = false;
        }
        else if (pid == 0)
        {
            execv(command[0], command);
            perror("Error w/ `execv()` - should not return! :");
            retVal = false;
        } else // Forked properly
        {
            retVal = true;
        }

        if (retVal)
        {
            if (waitpid(pid, &status, 0) == -1)
            {
                fprintf(stderr, "waitpid() failed with status: %d\n", status);
                retVal = false;
            }
            else if (WIFEXITED(status))
            {
                retVal = WEXITSTATUS(status) == 0 ? true : false;
            }
            else if (WIFSIGNALED(status))
            {
                fprintf(stdout, "Terminated on signal: %d\n", WTERMSIG(status)); 
                retVal = false;
            }
        }
    }

    va_end(args);

    return retVal;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

// /*
//  * TODO
//  *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
//  *   redirect standard out to a file specified by outputfile.
//  *   The rest of the behaviour is same as do_exec()
//  *
// */

    pid_t pid;
    int status;
    bool retVal = true;

    // Create/open new file
    int fd = open(outputfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("File open() failed: ");

        // Exit early to avoid another if {} nest
        va_end(args);
        return false;
    }

    // Non-obvious from instructions, but accept only absolute paths.
    if (!strchr(command[0], '/'))
    {
        retVal = false;
    }
    else
    {
        pid = fork();

        if (pid == -1)
        {
            perror("fork() failed: ");
            retVal = false;
        }
        else if (pid == 0)
        {
            // From referenced stackoverflow link:
            if (dup2(fd, 1) < 0)
            {
                perror("dup2()");    
                retVal = false;
            }
            else
            {
                close(fd); // parent
                execv(command[0], command);
                perror("Error w/ `execv()` - should not return! :");
                retVal = false;
            }
        }
        else
        {
            // Close the file in the parent
            close(fd);
            fd = 0;
        }

        if (retVal)
        {
            if (waitpid(pid, &status, 0) == -1)
            {
                fprintf(stderr, "waitpid() failed with status: %d\n", status);
                retVal = false;
            }
            else if (WIFEXITED(status))
            {
                retVal = WEXITSTATUS(status) == 0 ? true : false;
            }
            else if (WIFSIGNALED(status))
            {
                fprintf(stderr, "Terminated on signal: %d\n", WTERMSIG(status)); 
                retVal = false;
            }
        }
    }

    va_end(args);

    return retVal;
}
