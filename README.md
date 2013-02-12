18799
=====

Must have portaudio installed on your computer to compile. Once portaudio is
configured, the package is self compiling using the make file.

Project is segmented into packages. Makefile autodetects all c files and main
files to determine what executables to compile. Any c files named xxx\_main.c
will be interpreted as a binary and will be compiled to bin/xxx. All
executables assume they will be called from project root, i.e. bin/exec.
