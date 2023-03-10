##### Copyright 2020 Darius Neatu (neatudarius@gmail.com)
# README check

## View as webpage
```
sudo pip3 install grip
grip  README.md
# open http://localhost:6419/
```

## Description
This is the checker used to automatically grade homeworks at PC.

The name of the checker is `check`. It is written in `Python 3.6`.


## What does it do?
It will do the multiples steps.

All steps marked with `[STOP]` are required. If one failed the checker will stop.
All steps marked with `[OPTIONAL]` are optional and can be disabled from config.

1.  `deps`: Check if all dependencies are installed on local system in order to build/run/grade the homerwork.

2. `build`: Build homework.
	2.1. [STOP] `Makefile`: Check if `Makefile` exists.
	2.2. [STOP] `make`:  Run `make build` in order to build all binaries.
	2.3. [OPT]`warnings`:  If warnings are detected, a penalty to final grade is applied.

`Note`: This stage is using an explained `legend`:
	1. `UPS`: Ups, program crashed
		e.g null pointer dereference, negative or to big array/matrix indices
	2. `TLE`: Time Limit Exceed
		e.g. infinit loop or too slow
	3. `MLE`: Memory Limit Exceed
		e.g. too much allocated memory (in total or for some segments)
	3. `MEM_UPS`: Memory leaks or errors
		e.g. invalid memory access, unfreed dynamic-allocated arrays
	4. `WA`: Wrong Answer (wrong or partial output)
		e.g. output is missing or has other value
	5. `OK`: Everything is OK.

## Installation

The `install.sh` script can be used to install all dependencies for `check`.

Note: Please inspect  the script to see which are the requirements.

```
sudo ./install.sh
```

## Usage
- run entire homework

```
./check
```
