SLOT MACHINE
============

Line by line slot machine for the terminal.

![slot-machine](tty.gif)

## Building

A simple `make` `sudo make install` is enough. Requires a C99 compiler.

## Help message

    Usage: slot-machine [OPTS...] < INPUT
    
    Read lines from the standard input and print a slot machine style output
    to the standard error. Then print the pointed word.
    
      -m MS[:NUM],...  Set the delays between updates.
      -s SEL_COL       Set the pointer position (default: 0)
      -c COLS          Set the column count (default: 5)
      -C               Enable color output (default: false)
    
    Example: slot-machine -C -s 3 < examples/words
    Example: shuf exaples/words | slot-machine -C -s 3

## Collaborating

For making bug reports, feature requests, support or consulting visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)
