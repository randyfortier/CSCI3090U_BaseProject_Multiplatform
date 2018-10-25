# CSCI3090U Base Project (Multi-Platform Version)

This is a base OpenGL Project with makefiles for UNIX (supporting Linux and the Windows Subsystem for Linux), MacOS, and Windows (via NMake or MGW).

# How to Build

## Windows

Note:  There is also a Visual Studio 2017 Solution available.

### Using Windows Subsystem for Linux (WSL)

- ensure that you have GLEW and GLUT installed

```sh
$ sudo apt install libglew-dev libglfw3 libglfw3-dev
```

- build the project:

```sh
$ make -f Makefile.Unix
```

- run the application:

```sh
$ ./main
```

### Using Visual Studio Command Prompt

- GLEW and GLFW are included in the 'include' and 'lib' subfolders

```batch
> nmake /F Nmakefile.Windows
```

- run the application:

```batch
> main
```

## MacOS

Disclaimer:  The recommended platform is Windows.  Using any other platform may mean that technical support is not available.  I've taken a few hours to make it as seamless as possible on other platforms, but you are using it at your own risk.

- ensure that you have GLEW installed

e.g. using Homebrew:

```sh
$ brew install glew
```

- ensure that you have GLFW installed

e.g. using Homebrew:

```sh
$ brew install glfw3
```

- build the project:

```sh
$ make -f Makefile.MacOS
```

- run the application:

```sh
$ ./main
```

## Linux or other UNIX-based operating system

Note:  Driver support in Linux isn't the best.  Feel free to try it out, but if you are getting poor performance, it may be because some of the functionality is happening in software.

Disclaimer:  The recommended platform is Windows.  Using any other platform may mean that technical support is not available.  I've taken a few hours to make it as seamless as possible on other platforms, but you are using it at your own risk.

- ensure that you have GLEW and GLUT installed

e.g. on Ubuntu:
```sh
$ sudo apt install libglew-dev libglfw3 libglfw3-dev
```

- build the project:

```sh
$ make -f Makefile.Unix
```

- run the application:

```sh
$ ./main
```
