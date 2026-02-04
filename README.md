# techshell

`techshell` is a very simple shell for UNIX-like systems, written in C99.
It allows the user to execute commands found in the `PATH` environment
variable, change the directory using the `cd` built-in command, and do
basic input/output redirection.

## Compiling

To compile the shell, first clone the repository using git:

```sh
git clone https://github.com/lightdarkgreen/techshell.git
```

Then compile the file `techshell.c` with GCC (or another C99 compliant
compiler):

```sh
gcc --std=c99 -Wall -o techshell techshell.c
```

The resulting `techshell` binary can then be run:

```sh
./techshell
```

## Authors

Everything in this project was written by me, lightdarkgreen.  (If you know
me in real life, you know my real name; if you don't know me in real life,
my real name is none of your concern.)

## Generative AI

No portion of this project was created using generative AI.
