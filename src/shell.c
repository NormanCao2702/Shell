#define _XOPEN_SOURCE 700
#include "../include/msgs.h"
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_INPUT_SIZE 1024
#define MAX_HISTORY 10

char history[MAX_HISTORY][MAX_INPUT_SIZE];
int history_sequence[MAX_HISTORY];
int next_sequence = 0;

void add_to_history(const char *command, int sequence) {
  // int index = sequence % MAX_HISTORY;
  // if (index < 0) {
  // index += MAX_HISTORY;
  //}
  int history_index = sequence % MAX_HISTORY;
  strcpy(history[history_index], command);
  history_sequence[history_index] = sequence;
  // snprintf(history[sequence % MAX_HISTORY], );
  // history_sequence[sequence % MAX_HISTORY] = sequence;
}

void display_history() {
  int i, j;
  int history_count = 0;
  char output[MAX_INPUT_SIZE * 2];
  for (i = next_sequence - 1; i >= 0 && history_count < MAX_HISTORY; i--) {
    j = i % MAX_HISTORY;
    if (history_sequence[j] != -1) {
      snprintf(output, sizeof(output), "%d\t%s\n", history_sequence[j],
               history[j]);
      write(STDOUT_FILENO, output, strlen(output));
      history_count++;
    }
  }
}
void print_prompt() {
  char cwd[MAX_INPUT_SIZE];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    write(STDOUT_FILENO, cwd, strlen(cwd));
    write(STDOUT_FILENO, "$ ", 2);
  } else {
    write(STDOUT_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
  }
}

void sigint_handler(int sig) {
  // signal(SIGINT, sigint_handler);
  write(STDOUT_FILENO, FORMAT_MSG("\nhelp", HELP_HELP_MSG),
        strlen(FORMAT_MSG(HELP_HELP_MSG, "\nhelp")));

  // fflush(stdout);
  write(STDOUT_FILENO, FORMAT_MSG("cd", CD_HELP_MSG),
        strlen(FORMAT_MSG(CD_HELP_MSG, "cd")));

  // fflush(stdout);
  write(STDOUT_FILENO, FORMAT_MSG("exit", EXIT_HELP_MSG),
        strlen(FORMAT_MSG(EXIT_HELP_MSG, "exit")));

  // fflush(stdout);
  write(STDOUT_FILENO, FORMAT_MSG("pwd", PWD_HELP_MSG),
        strlen(FORMAT_MSG(PWD_HELP_MSG, "pwd")));

  // fflush(stdout);
  write(STDOUT_FILENO, FORMAT_MSG("history", HISTORY_HELP_MSG),
        strlen(FORMAT_MSG(HISTORY_HELP_MSG, "history")));
  fflush(stdout);
  print_prompt();
  //  credit goes to:
  //  https://www.geeksforgeeks.org/write-a-c-program-that-doesnt-terminate-when-ctrlc-is-pressed/
}

int main() {
  char input[MAX_INPUT_SIZE];
  int status;
  int argc;
  char *args[MAX_INPUT_SIZE];

  signal(SIGINT, sigint_handler);
  while (1) {
    while (waitpid(-1, &status, WNOHANG) > 0)
      ;

    print_prompt();

    ssize_t n = read(STDIN_FILENO, input, sizeof(input));

    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        write(STDOUT_FILENO, READ_ERROR_MSG, strlen(READ_ERROR_MSG));
        continue;
      }
    }

    input[n - 1] = '\0';

    add_to_history(input, next_sequence);
    char *token = strtok(input, " ");
    argc = 0;
    int background_process = 0;
    while (token != NULL) {
      args[argc] = token;
      if (strcmp(token, "&") == 0) {
        background_process = 1;
        args[argc] = NULL;
      }
      token = strtok(NULL, " ");
      argc++;
    }

    args[argc] = NULL;
    if (argc == 0) {
      continue;
    }

    if (strcmp(args[0], "exit") == 0) {
      if (argc > 1) {
        write(STDERR_FILENO, FORMAT_MSG("exit", TMA_MSG),
              strlen(FORMAT_MSG("exit", TMA_MSG)));
      } else {
        exit(0);
      }
    } else if (strcmp(args[0], "history") == 0) {
      // add_to_history(input, next_sequence);
      next_sequence++;
      display_history();
      // add_to_history(input, next_sequence);
      // next_sequence++;
    } else if (argc > 0 && strcmp(args[0], "help") == 0) {
      if (argc == 1) {
        write(STDOUT_FILENO, FORMAT_MSG("cd", CD_HELP_MSG),
              strlen(FORMAT_MSG("cd", CD_HELP_MSG)));
        write(STDOUT_FILENO, FORMAT_MSG("pwd", PWD_HELP_MSG),
              strlen(FORMAT_MSG("pwd", PWD_HELP_MSG)));
        write(STDOUT_FILENO, FORMAT_MSG("exit", EXIT_HELP_MSG),
              strlen(FORMAT_MSG("exit", EXIT_HELP_MSG)));
        write(STDOUT_FILENO, FORMAT_MSG("help", HELP_HELP_MSG),
              strlen(FORMAT_MSG("exit", HELP_HELP_MSG)));
        write(STDOUT_FILENO, FORMAT_MSG("history", HISTORY_HELP_MSG),
              strlen(FORMAT_MSG("history", HISTORY_HELP_MSG)));
      } else if (argc == 2) {
        if (strcmp(args[1], "cd") == 0) {
          write(STDOUT_FILENO, FORMAT_MSG("cd", CD_HELP_MSG),
                strlen(FORMAT_MSG("cd", CD_HELP_MSG)));
        } else if (strcmp(args[1], "pwd") == 0) {
          write(STDOUT_FILENO, FORMAT_MSG("pwd", PWD_HELP_MSG),
                strlen(FORMAT_MSG("pwd", PWD_HELP_MSG)));
        } else {
          char msg[100];
          snprintf(msg, sizeof(msg), "%s: %s\n", args[1], EXTERN_HELP_MSG);
          // write(STDOUT_FILENO, FORMAT_MSG(input, EXTERN_HELP_MSG),
          // strlen(FORMAT_MSG(input, EXTERN_HELP_MSG)));
          write(STDOUT_FILENO, msg, strlen(msg));
        }
      } else {
        write(STDOUT_FILENO, FORMAT_MSG("help", TMA_MSG),
              strlen(FORMAT_MSG("help", TMA_MSG)));
      }
      // add_to_history(input, next_sequence);
      next_sequence++;
    } else if (strcmp(args[0], "pwd") == 0) {
      if (argc > 1) {
        write(STDERR_FILENO, FORMAT_MSG("pwd", TMA_MSG),
              strlen(FORMAT_MSG("pwd", TMA_MSG)));
      } else {
        char cwd[MAX_INPUT_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          write(STDOUT_FILENO, cwd, strlen(cwd));
          write(STDOUT_FILENO, "\n", 1);
        } else {
          write(STDOUT_FILENO, GETCWD_ERROR_MSG, strlen(GETCWD_ERROR_MSG));
          write(STDOUT_FILENO, "\n", 1);
        }
      }
      // add_to_history(input, next_sequence);
      next_sequence++;
    } else if (strcmp(args[0], "cd") == 0) {
      if (argc > 2) {
        write(STDERR_FILENO, FORMAT_MSG("cd", TMA_MSG),
              strlen(FORMAT_MSG("cd", TMA_MSG)));
      } else {
        char *path = args[1];
        if (path != NULL) {
          if (strcmp(path, "~") == 0) {
            struct passwd *pw = getpwuid(getuid());
            if (pw != NULL) {
              path = pw->pw_dir;
            }
          } else if (strcmp(path, "-") == 0) {
            path = getenv("OLDPWD");
            // chdir(prevcwd);
          }
        }
        if (path != NULL && chdir(path) == -1) {
          write(STDERR_FILENO, FORMAT_MSG("cd", CHDIR_ERROR_MSG),
                strlen(FORMAT_MSG("cd", CHDIR_ERROR_MSG)));
        } else {
          char current_dir[PATH_MAX];
          if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
            setenv("OLDPWD", current_dir, 1);
          }
        }
      }
      // add_to_history(input, next_sequence);
      next_sequence++;
    } else {
      // add_to_history(input, next_sequence);
      next_sequence++;
      pid_t child_pid = fork();
      if (child_pid < 0) {
        write(STDERR_FILENO, FORK_ERROR_MSG, strlen(FORK_ERROR_MSG));
      } else if (child_pid == 0) // CHILD PROCESS
      {
        execvp(input, args);
        write(STDERR_FILENO, FORMAT_MSG("shell", EXEC_ERROR_MSG),
              strlen(FORMAT_MSG("shell", EXEC_ERROR_MSG)));
        exit(0);

      } else if (child_pid > 0) // parent process
      {
        if (!background_process) {
          wait(&status);
          if (status) {
            write(STDOUT_FILENO, WAIT_ERROR_MSG, strlen(WAIT_ERROR_MSG));
          }
        }
      }
    }
  }
  return 0;
}
