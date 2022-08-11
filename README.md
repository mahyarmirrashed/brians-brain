# Brian's Brain

[Brian's Brain](https://www.wikiwand.com/en/Brian%27s_Brain) is a cellular automaton similar to [Conway's Game of Life](https://www.wikiwand.com/en/Conway%27s_Game_of_Life).

This code will generate a video of the Brian's Brain automaton for your viewing pleasure.

## Automaton Rules

Each cell of the automaton takes three different states: ready (off), firing (on), and refractory (dying). Cells turn on if they had exactly two neighbors in their [Moore neighborhood](https://www.wikiwand.com/en/Moore_neighborhood). Otherwise, live cells become dying cells, dying cells become dead cells, and dead cells stay dead cells.

## Build and Running the Program

To build the program, simply run `make` with the supplied `Makefile`. Then, run the program like a normal binary `./main`.

This program uses OpenMP for accelerating frame generation.
