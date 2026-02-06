# techshell

`techshell` is a very simple shell for UNIX-like systems, written in C99.
It allows the user to execute commands found in the `PATH` environment
variable, change the directory using the `cd` built-in command, and do
basic input/output redirection.

## Compiling

To compile the shell, clone the git repository and run `make` in the root
directory of the repository:

```sh
git clone https://github.com/lightdarkgreen/techshell.git
cd techshell
make
```

Alternatively, if you don't want to use `make`, you can use the following
sequence of commands in the root directory of the repository to manually
compile the shell:

```sh
mkdir obj
gcc --std=c99 -Wall -O3 -c -o obj/cd.o src/cd.c
gcc --std=c99 -Wall -O3 -c -o obj/error.o src/error.c
gcc --std=c99 -Wall -O3 -c -o obj/memory.o src/memory.c
gcc --std=c99 -Wall -O3 -c -o obj/parser.o src/parser.c
gcc --std=c99 -Wall -O3 -c -o obj/prompt.o src/prompt.c
gcc --std=c99 -Wall -O3 -c -o obj/runner.o src/runner.c
gcc --std=c99 -Wall -O3 -c -o obj/techshell.o src/techshell.c
gcc --std=c99 -Wall -O3 -o techshell obj/*.o
```

After compiling the shell, it will be found in the root directory of the
repository.  It can be run using the command:

```sh
./techshell
```

## Layout

All of the source files are in the `src` subdirectory.  The main file is
`techshell.c`, which uses functions defined in the other files to do most
of its work.  All of the source files other than `techshell.c` come in `.c`
and `.h` pairs; e.g. `parser.c` and `parser.h`; the `.h` files are headers
containing prototypes for the functions defined in the `.c` files.

The `cd` command is defined in `cd.c`.

Error reporting is defined in `error.c`.

Wrappers for memory allocation are defined in `memory.c`.  These call
standard functions like `malloc` and `realloc`, but check for out of memory
errors; while these are rare, they still need to be handled.

Parsing and line input are defined in `parser.c`.

Prompt display logic is defiend in `prompt.c`.

Spawning of child processes to run the commands is handled in `runner.c`.

## Authors

Everything in this project was written by me, lightdarkgreen.  (If you know
me in real life, you know my real name; if you don't know me in real life,
my real name is none of your concern.)

## Generative AI

No portion of this project was created using generative AI.
