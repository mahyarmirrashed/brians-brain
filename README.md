# Brian's Brain

[Brian's Brain](https://www.wikiwand.com/en/Brian%27s_Brain) is a cellular automaton similar to [Conway's Game of Life](https://www.wikiwand.com/en/Conway%27s_Game_of_Life).

This code will generate a video of the Brian's Brain automaton for your viewing pleasure.

## Automaton Rules

Each cell of the automaton takes three different states: ready (off), firing (on), and refractory (dying). Cells turn on if they had exactly two neighbors in their [Moore neighborhood](https://www.wikiwand.com/en/Moore_neighborhood). Otherwise, live cells become dying cells, dying cells become dead cells, and dead cells stay dead cells.

## Build and Running the Program

To build the program, simply run `make` with the supplied `Makefile`. Then, run the program like a normal binary `./main`.

This program uses OpenMP for accelerating frame generation.

## Generation Options

This program comes equipped with options to generate to any resolution and to alter the number of frames generated (locked at 30 frames per second, however).

View the program's help options with `./main --help`.

## Performance

An interesting observation that I made is that 75% of the CPU time goes to writing the frames to the video stream. Since this is memory-bound, I did not know any way of making it faster.
