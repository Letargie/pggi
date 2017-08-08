<img src="logo!.png" align="right" />

# PGGI

PHP extension providing a small langage binding for GTK+ 3 toolkit allowing to write GUI Applications.
I made this extension in order to learn how to make php extension and learn GTK, therefore this extension is highly experimental but if you want try it, I would love receiving feedbacks.
For now there isn't much but I intend to add more to it.

## Table of content

- [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installing](#installing)
- [Running the tests](#running-the-tests)

## Getting Started

### Prerequisites

As a prerequisite, you should have a CLI binary (command-line) version of PHP
installed along with the header files and development scripts.

For now, PGGI needs PHP 7.1 or greater to run, and to build. (could work with php 7.0, haven't try it )

PGGI supports GTK+ 3.2 or greater (I will have to make some tweaking for 3.0 and 3.1 but it could work in the future)

### Installing

For now you have to install it manually.

In terminal move to the pggi directory and run.

```
phpize
```
Followed by

```
./configure
```

Finally run

```
./make
```

To test the extension, try running the exemples scripts in the exemples directory.

You can also check my other repositories where I put applications using pggi, that a good way to see how it works and have a nice demo.

## Running the tests

you can launch tests by running the file run-tests.php

For now there isn't any.

## Contact

if you want to contact me, you can send me an email at <valentin@famillecollet.com>


