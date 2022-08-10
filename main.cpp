/**
 * @file main.cpp
 * @author Mahyar Mirrashed (mirrashm@myumanitoba.ca)
 * @brief Generate frames from the Brian's Brain cellular automaton.
 * @version 0.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Mahyar Mirrashed
 *
 */

#include <argp.h>
#include <stdint.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------

// default to 60 seconds of video
#define DEFAULT_FRAME_COUNT 1800
// default standard high-definition (HD) display resolution (720p)
#define DEFAULT_COLUMNS 1280
#define DEFAULT_ROWS    720

//-----------------------------------------------------------------------------
// ARGUMENT PARSER SETUP
//-----------------------------------------------------------------------------

const char *argp_program_version = "brains-brain 0.0.0";
const char *argp_program_bug_address = "<mirrashm@myumanitoba.ca>";

static char doc[] = "Brian's Brian cellular automaton video generator.";

static struct argp_option options[] = {
  {.name = "frames",
   .key = 'f',
   .arg = "FRAMES",
   .flags = OPTION_ARG_OPTIONAL,
   .doc = "Number of frames to generate"},
  {.name = "rows",
   .key = 'r',
   .arg = "ROWS",
   .flags = OPTION_ARG_OPTIONAL,
   .doc = "Number of rows in each frame"},
  {.name = "columns",
   .key = 'c',
   .arg = "COLUMNS",
   .flags = OPTION_ARG_OPTIONAL,
   .doc = "Number of columns in each frame"},
  {0},
};

typedef struct {
  int frames;
  int rows;
  int colums;
} arguments;

static arguments args;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------

int main(const int argc, const char **argv) {
  return EXIT_SUCCESS;
}
