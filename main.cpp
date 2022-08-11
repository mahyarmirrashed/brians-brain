/**
 * @file main.cpp
 * @author Mahyar Mirrashed (mirrashm@myumanitoba.ca)
 * @brief Generate frames from the Brian's Brain cellular automaton.
 * @version 0.1.0
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
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------

// default to 60 seconds of video
#define DEFAULT_FRAME_COUNT 1800
// default standard high-definition (HD) display resolution (720p)
#define DEFAULT_COLUMNS 1280
#define DEFAULT_ROWS    720

static const cv::Vec3b ON = cv::Vec3b({255, 255, 255});
static const cv::Vec3b DYING = cv::Vec3b({255, 0, 0});
static const cv::Vec3b OFF = cv::Vec3b({0, 0, 0});

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

int main(int argc, char **argv);
static void brain(const cv::Mat &__restrict__ in, cv::Mat &__restrict__ out);
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
int main(int argc, char **argv) {
  // variables used for video generation
  int i, j;
  cv::VideoWriter video;
  cv::Mat prev, curr;

  // set default argument values
  args.frames = DEFAULT_FRAME_COUNT;
  args.columns = DEFAULT_COLUMNS;
  args.rows = DEFAULT_ROWS;

  // parse arguments from argument vector
  argp_parse(&argp, argc, argv, 0, 0, &args);

  // create video stream (receives bytes from input matrix)
  video.open(
    "automaton.avi", cv::VideoWriter::fourcc('F', 'F', 'V', '1'), 30.0,
    cv::Size(args.columns, args.rows)
  );

  // video frame size initialization
  prev = cv::Mat(args.rows, args.columns, CV_8UC3);
  curr = cv::Mat(args.rows, args.columns, CV_8UC3);

  // randomly seed frame for interesting initialization
  // TODO

  for (i = 0; i < args.frames; i += 1) {
    // save current frame
    video << curr;
    // migrate current frame to previous frame
    curr.copyTo(prev);
    // calculate next frame of automaton
    brain(prev, curr);
  }

  return EXIT_SUCCESS;
}

/**
 * @brief Model of Brian's Brain implemented as cleanly as possible.
 *
 * @param in Previous frame of Brian's Brain.
 * @param out New, current frame of Brian's Brain.
 */
static void brain(const cv::Mat &__restrict__ in, cv::Mat &__restrict__ out) {
  int i, j, k, l;
  int tot;

  for (i = 0; i < in.rows; i += 1)
    for (j = 0; j < in.cols; j += 1)
      if (in.at<cv::Vec3b>(i, j) == ON)
        out.at<cv::Vec3b>(i, j) = DYING;
      else if (in.at<cv::Vec3b>(i, j) == DYING)
        out.at<cv::Vec3b>(i, j) = OFF;
      else {
        // search Moore neighborhood for live cells
        for (tot = 0, k = -1; k < 2; k += 1)
          if (i + k >= 0 && i + k < in.rows)
            for (l = -1; l < 2; l += 1)
              if (j + l >= 0 && j + l < in.cols)
                if (in.at<cv::Vec3b>(i + k, j + l) == ON)
                  tot += 1;

        // automaton rule dictates turning on only if two neighbor cells are on
        out.at<cv::Vec3b>(i, j) = (tot == 2) ? ON : OFF;
      }
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
  } else {
    rc = ARGP_ERR_UNKNOWN;
  }

  return rc;
}
