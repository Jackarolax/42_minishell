# Tasks

### Chores

- [ ] clean up the header
- [ ] nicer prompt maybe
- [X] norminette the code

### TODOs

- [ ] append built in commands to execution
- [X] allow execution from path "./bin/..." by checking after the . or ..
- [X] make the cleanup function
- [ ] check for input errors in redirections, check for every error tbh.
- [X] free the buffer when CTRL C is pressed
- [X] Error when path points to a folder rather than executable binary
- [X] `ls > out1.txt > out2.txt` is not creating both files, only out2.txt
- [X] There is a leak for `cat < nonexistent.txt` exits safely tho
- [X] Something is happening in `cat /dev/urandom | head -n 5`
- [ ] Heredoc

### Parsing
- [X] Read input
  - [X] Handle signals here maybe
- [X] Lexer
  - [X] Get line
  - [X] Split line into tokens
  - [X] Handle quotes `'`,`"`
  - [X] Handle escape characters
  - [X] Handle(input[i] == '>' && input[i + 1] == '>') metacharacters
    - Remarks: Characters like redirects and the heredoc is parsed as a token on a list
- [X] Parsing
  - [X] Build command structures from tokens
  - [X] Handle Syntax errors

### Commands
- [ ] Built-in commands

  - [x] `echo` with `-n`
    - Remarks: Function expects only tokens after echo including echo as argv[0].
    ```C
    void ft_echo(int argc, char **argv);
    ```

  - [x] `cd` only with a relative or absolute path
    - Remarks: Function expects only tokens after cd including cd as argv[0]. Expects argc to be 1.
    ```C
    void ft_cd(t_env *env,int argc, char **argv);
    ```

  - [x] `pwd`
    ```C
    void ft_pwd(void);
    ```
    - There is an environment variable called `OLDPWD` hmm...
  - [x] `export`
    ```C
    void	ft_export(t_env *env, int argc, char **argv);
    ```
  - [x] `unset`
    ```C
    void	ft_unset(t_env **env_p, int argc, char **argv);
    ```
  - [x] `env`
    ```C
    void	ft_env(t_env *env, int argc, char **argv);
    ```
  - [x] `exit`
    ```C
    void	ft_exit(t_minishell *data_p);
    ```
- [X] Pipes and Redirection
  - [X] `|` output of one command to the next command
  - [X] `>` redirect output
  - [X] `<` redirect input
  - [X] `<<` should be given a delimiter, it will read the input until the delimiter is seen. It doesn't have to update history.
  - Note: do check manual and heredoc
- [X] External commands
  - [ ] Handle `$ENV` outputting proper commands
  - [ ] Handle error for non-existing commands
    - Remarks: to test

### Interactive mode
- [X] `ctrl + D` exits the shell
- [X] `ctrl + C` displays a new prompt
- [X] `ctrl + \` does nothing

### Extra
- [ ] Describe functions
- [ ] Wrap-up `README.md`

### Notes

```C
char *readline(char *)
```
Readline will have leaks when used. No need to fix.

**Team has decided on NO READLINE**

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
