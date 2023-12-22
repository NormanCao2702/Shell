# Shell

A simple shell like bash or zsh. Intended to run on Unix-like system (including macOS).

## Features

- Enter any commands like any other shell and see the result.
- Also handle Ctrl + C key press, you need to enter exit in order to quit the program.
- Shell internal commands:
    - [x] cd
    - [x] pwd
    - [x] help
    - [x] exit
    - [] history (on going updated)

```
/Users/tran/Desktop/Shell/build$ ls -al
total 3784
drwxr-xr-x@ 14 tran  staff      448 Dec 22 00:57 .
drwxr-xr-x@ 12 tran  staff      384 Dec 22 00:56 ..
-rw-r--r--@  1 tran  staff    21644 Dec 22 00:56 CMakeCache.txt
drwxr-xr-x@ 14 tran  staff      448 Dec 22 00:57 CMakeFiles
-rw-r--r--@  1 tran  staff      374 Dec 22 00:56 CTestTestfile.cmake
-rw-r--r--@  1 tran  staff    11172 Dec 22 00:56 Makefile
drwxr-xr-x@  5 tran  staff      160 Dec 22 00:56 _deps
drwxr-xr-x@  2 tran  staff       64 Dec 22 00:56 bin
-rw-r--r--@  1 tran  staff     1705 Dec 22 00:56 cmake_install.cmake
drwxr-xr-x@  6 tran  staff      192 Dec 22 00:57 lib
-rwxr-xr-x@  1 tran  staff    34914 Dec 22 00:56 shell
-rwxr-xr-x@  1 tran  staff  1836423 Dec 22 00:56 shell_test
-rw-r--r--@  1 tran  staff      211 Dec 22 00:56 shell_test[1]_include.cmake
-rw-r--r--@  1 tran  staff    11776 Dec 22 00:56 shell_test[1]_tests.cmake
/Users/tran/Desktop/Shell/build$ ^C
help: show help information
cd: change the current directory
exit: exit the shell
pwd: print the current directory
history: print the history of commands
/Users/tran/Desktop/Shell/build$ 
```

## Quick note

- The program is signal safe, that means using all non-buffered I/O like read() and write() to print message and read user input.
- More information, please take a look at `signal-safety(7)` man page.

## Build instruction

- Assuming you already had cmake and make file configuration.
- Either clone or download from this repo, then, go to the build folder, type these following command:

`./shell`


