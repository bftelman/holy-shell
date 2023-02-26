#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "hshell.h"
#include "color.h"
#include "lol.h"

#define hshell_BUFSIZE 1024
#define hshell_TOKEN_BUFSIZE 64
#define hshell_TOKEN_DELIMS " \t\r\n\a"

char *history[1024];
int history_count = 0;

void add_line_to_history(const char *line)
{
    if (history_count < 1024)
    {
        history[history_count++] = strdup(line);
    }
    else
    {
        free(history[0]);
        for (int index = 1; index < 1024; index++)
        {
            history[index - 1] = history[index];
        }
        history[1024 - 1] = strdup(line);
    }
}

char cwd[1024];

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "color",
    "lol"};

int (*builtin_func[])(char **) = {
    &hshell_cd,
    &hshell_help,
    &hshell_exit,
    &color,
    &lol};

int hshell_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

void hshell_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        getcwd(cwd, sizeof(cwd));
        printf("[ %s ]", cwd);
        printf("> ");
        line = hshell_read_line();
        add_line_to_history(line);
        args = hshell_split_line(line);
        status = hshell_exec(args);

        free(line);
        free(args);
    } while (status);
}

char *hshell_read_line(void)
{
    int bufsize = hshell_BUFSIZE;
    int position = 0;
    int c;
    char *buffer = malloc(sizeof(char) * bufsize);

    if (!buffer)
    {
        fprintf(stderr, "hshell: allocation error!\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int index = rand() % 7;
        char *color = get_color(index);
        printf("%s", color);
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }

        position++;
    }

    if (position >= bufsize)
    {
        bufsize += hshell_BUFSIZE;
        buffer = realloc(buffer, bufsize);
        if (!buffer)
        {
            fprintf(stderr, "hshell: allocation error!\n");
            exit(EXIT_FAILURE);
        }
    }
}

char **hshell_split_line(char *line)
{
    int bufsize = hshell_TOKEN_BUFSIZE, position = 0;
    char **tokens = malloc(sizeof(char *) * bufsize);
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "hshell: allocation error!\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, hshell_TOKEN_DELIMS);

    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += hshell_TOKEN_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "hshell: allocation error!\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, hshell_TOKEN_DELIMS);
    }

    tokens[position] = NULL;
    return tokens;
}

int hshell_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("hshell");
        }

        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("hshell");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int hshell_exec(char **args)
{
    int i;
    if (args[0] == NULL)
    {
        return 1;
    }

    for (i = 0; i < hshell_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return hshell_launch(args);
}

int hshell_cd(char **args)
{
    if (args[1] == NULL)
    {
        chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("hshell");
        }
    }
    return 1;
}

int hshell_help(char **args)
{
    int i;
    printf("Telman's HOLY hshell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < hshell_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    printf(" _            __                  _             __   __ \n");
    printf("(_)          (__)           ____ (_)      ____ (__) (__)\n");
    printf("(_)__    ___  (_) _   _    (____)(_)__   (____) (_)  (_) \n");
    printf("(____)  (___) (_)(_) (_)   (_)__ (____) (_)_(_) (_)  (_)\n");
    printf("(_) (_)(_)_(_)(_)(_)_(_)    _(__)(_) (_)(__)__  (_)  (_) \n");
    printf("(_) (_) (___)(___)(____)   (____)(_) (_) (____)(___)(___)\n");
    printf("                   __(_)\n");
    printf("                  (___)\n");
    printf("\t\t\t\tV1\n");
    return 1;
}

int hshell_exit(char **args)
{
    FILE *HOLY_HISTORY;
    HOLY_HISTORY = fopen("holy_history", "w+");
    for (int i = 0; i < history_count; i++)
    {
        //fprintf("%d\n", i);
        fprintf(HOLY_HISTORY, "%s\n", history[i]);
    }
    fclose(HOLY_HISTORY);

    printf(WHT);
    return 0;
}
