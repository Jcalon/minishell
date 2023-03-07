# minishell

The goal was recoding our own little bash. A program capable of parsing a prompt and launching executables with arguments, along with a few built-in functions.

### Steps to run the minishell
```bash
$> make

$> ./minishell
```

### Things to handle
- `Single quotes ''`  
- `Double quotes ""`
- `Redirections <, >, >>`
- `environment variable $`
- `pipes: cmd | cmd`
- `builtins`:  
  - `env, export, unset`
  - `echo`
  - `cd, pwd`
  - `exit`
 - `$?`
- Finally, you can use Ctrl-C to interrupt and Ctrl-\ to quit a program, as well as Ctrl-D to throw an EOF, same as in bash.
