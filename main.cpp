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
#include <errno.h>
#include <limits.h>
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

static char args_doc[] = "";
static char doc[] = "Brian's Brian cellular automaton video generator.";

static struct argp_option options[] = {
  {.name = "frames",
   .key = 'f',
   .arg = "FRAMES",
   .flags = OPTION_ARG_OPTIONAL,
   .doc = "Number of frames to generate"},
  {.name = "columns",
   .key = 'c',
   .arg = "COLUMNS",
   .flags = OPTION_ARG_OPTIONAL,
   .doc = "Number of columns in each frame"},
  {.name = "rows",
   .key = 'r',
   .arg = "ROWS",
   .flags = OPTION_ARG_OPTIONAL,
   .doc = "Number of rows in each frame"},
  {0},
};

typedef struct {
  int frames;
  int columns;
  int rows;
} arguments;

static arguments args;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------

int main(const int argc, const char **argv);
static error_t parse_opt(int key, char *arg, struct argp_state *state);

//-----------------------------------------------------------------------------
// ARGUMENT PARSER INITIALIZATION
//-----------------------------------------------------------------------------

static struct argp argp {
  .options = options, .parser = parse_opt, .args_doc = args_doc, .doc = doc,
  .children = NULL,
};

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief Generate a video of the Brian's Brain automaton.
 *
 * @param argc Number of arguments passed.
 * @param argv Argument vector of arguments.
 * @return int Return code status of program.
 */
int main(const int argc, const char **argv) {
  // set default argument values
  args.frames = DEFAULT_FRAME_COUNT;
  args.columns = DEFAULT_COLUMNS;
  args.rows = DEFAULT_ROWS;

  return EXIT_SUCCESS;
}

/**
 * @brief Parse arguments passed to program and perform basic error checking.
 *
 * @param key Integer specifying which option this is.
 * @param arg For option key, this is NULL if no key exists.
 * @param state Pointer to argp state containing useful information about the
 * parsing state.
 * @return error_t Return code to argp library.
 */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  arguments *sargs = (arguments *)state->input;
  error_t rc = EXIT_SUCCESS;

  if (key == 'f' || key == 'c' || key == 'r') {
    // convert argument to long integer
    char *endptr;
    unsigned long value = strtoul(arg, &endptr, 10);

    // check conversion errors
    if (errno == EINVAL)
      argp_failure(state, 1, 0, "given base contains unsupported value");
    else if (errno == ERANGE)
      argp_failure(state, 1, 0, "resulting value was out of range");

    // perform more detailed error checking based on specific key
    if (key == 'f') {
      args.frames = value;
    } else if (key == 'c') {
      if (value >= USHRT_MAX)
        argp_failure(state, 1, 0, "too many columns");
      else
        args.columns = value;
    } else if (key == 'r') {
      if (value >= USHRT_MAX)
        argp_failure(state, 1, 0, "too many rows");
      else
        args.rows = value;
    }
  }
}
