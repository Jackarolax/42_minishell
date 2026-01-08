# Tasks

### Parsing
- [X] Read input
  - [ ] Handle signals here maybe
- [X] Lexer
  - [X] Get line
  - [X] Split line into tokens
  - [ ] Handle quotes `'`,`"`
  - [ ] Handle escape characters
  - [ ] Handle metacharacters
- [ ] Parsing
  - [ ] Build command structures from tokens
  - [ ] Handle Syntax errors

### Commands
- [ ] Built-in commands
  - [x] `echo` with `-n`
    - Remarks: Function expects only tokens after echo including echo as argv[0].
    ```C
    void ft_echo(char **argv);
    ```

  - [x] `cd` only with a relative or absolute path
  - Remarks: Function expects only tokens after cd including cd as argv[0]. Expects argc to be 1.
    ```C
    void ft_cd(int argc, char **argv);
    ```
  - [x] `pwd`
    ```C
    void ft_pwd();
    ```
  - [ ] `export`
  - [ ] `unset`
  - [ ] `env`
  - [ ] `exit`
- [ ] Pipes and Redirection
  - [ ] `|` output of one command to the next command
  - [ ] `>` redirect output
  - [ ] `<` redirect input
  - [ ] `<<` should be given a delimiter, it will read the input until the delimiter is seen. It doesn't have to update history.
  - Note: do check manual and heredoc
- [ ] External commands
  - [ ] Handle `$ENV` outputting proper commands
  - [ ] Handle error for non-existing commands

### Interactive mode
- [ ] `ctrl + D` exits the shell
- [ ] `ctrl + C` displays a new prompt
- [ ] `ctrl + \` does nothing

### Extra
- [ ] Describe functions
- [ ] Wrap-up `README.md`

### Notes

```C
char *readline(char *)
```
Readline will have leaks when used. No need to fix.

Available functions:

`readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs`
