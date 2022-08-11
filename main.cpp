/**
 * @file main.cpp
 * @author Mahyar Mirrashed (mirrashm@myumanitoba.ca)
 * @brief Generate frames from the Brian's Brain cellular automaton.
 * @version 0.2.2
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
#include <sys/ioctl.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------

// default to 60 seconds of video
#define DEFAULT_FRAME_COUNT 1800
// default standard high-definition (HD) display resolution (720p)
#define DEFAULT_COLUMNS 1280
#define DEFAULT_ROWS    720

// default seeding area for random initialization (always square)
#define DEFAULT_SEED_AREA 0.4

// progress bar constants
#define MAX_PROGRESS 100
#define PROGRESS_BAR                                                           \
  "##########################################################################" \
  "##########################"
#define PROGRESS_BAR_BLANK                                                     \
  "                                                                          " \
  "                          "

static const cv::Vec3b ON = cv::Vec3b({255, 255, 255});
static const cv::Vec3b DYING = cv::Vec3b({255, 0, 0});
static const cv::Vec3b OFF = cv::Vec3b({0, 0, 0});

//-----------------------------------------------------------------------------
// ARGUMENT PARSER SETUP
//-----------------------------------------------------------------------------

const char *argp_program_version = "brains-brain 0.2.2";
const char *argp_program_bug_address = "<mirrashm@myumanitoba.ca>";

static char args_doc[] = "";
static char doc[] = "Brian's Brian cellular automaton video generator.";

static struct argp_option options[] = {
  {.name = "frames",
   .key = 'f',
   .arg = "FRAMES",
   .flags = 0,
   .doc = "Number of frames to generate",
   .group = 0},
  {.name = "columns",
   .key = 'c',
   .arg = "COLUMNS",
   .flags = 0,
   .doc = "Number of columns in each frame",
   .group = 0},
  {.name = "rows",
   .key = 'r',
   .arg = "ROWS",
   .flags = 0,
   .doc = "Number of rows in each frame",
   .group = 0},
  {},
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
static void display_progress(const int progress);
static error_t parse_opt(int key, char *arg, struct argp_state *state);

//-----------------------------------------------------------------------------
// ARGUMENT PARSER INITIALIZATION
//-----------------------------------------------------------------------------

static struct argp argp {
  .options = options, .parser = parse_opt, .args_doc = args_doc, .doc = doc,
  .children = NULL, .help_filter = NULL, .argp_domain = NULL
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
  int size;
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
  std::srand(clock());
  size = std::min(args.rows, args.columns) * DEFAULT_SEED_AREA;

  for (i = (args.rows - size) / 2; i < (args.rows + size) / 2; i += 1)
    for (j = (args.columns - size) / 2; j < (args.columns + size) / 2; j += 1)
      curr.at<cv::Vec3b>(i, j) = std::rand() & 1 ? ON : OFF;

  // hide cursor when printing progress
  printf("\33[?25l");

  for (i = 0; i < args.frames; i += 1) {
    // display progress bar
    display_progress((i * 100) / args.frames);
    // save current frame
    video << curr;
    // migrate current frame to previous frame
    curr.copyTo(prev);
    // calculate next frame of automaton
    brain(prev, curr);
  }

  // report that simulation generation is complete
  printf("\33[2K\rCompleted generating the simulation! Enjoy!\n");

  // re-enable cursor after program
  printf("\33[?25h");

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

#pragma omp parallel for
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
 * @brief Display progress bar of how much of generation has happened.
 *
 * @param progress Amount of progress.
 */
static void display_progress(const int progress) {
  if (progress < 0 || progress > MAX_PROGRESS)
    return;

  struct winsize sz;
  int rsz_progress;

  // get window size from tty
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);

  // resize progress to match new progress bar size
  rsz_progress = (progress * sz.ws_row) / MAX_PROGRESS;

  // print progress bar using this weird C stynax
  printf(
    "\33[2K\rGenerating: [%.*s %.*s] %d%%", rsz_progress, PROGRESS_BAR,
    sz.ws_row - rsz_progress, PROGRESS_BAR_BLANK, progress
  );

  // flush progress bar to user
  // clearing terminals is usually buffered
  fflush(stdout);
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
      sargs->frames = value;
    } else if (key == 'c') {
      if (value >= USHRT_MAX)
        argp_failure(state, 1, 0, "too many columns");
      else
        sargs->columns = value;
    } else if (key == 'r') {
      if (value >= USHRT_MAX)
        argp_failure(state, 1, 0, "too many rows");
      else
        sargs->rows = value;
    }
  } else {
    rc = ARGP_ERR_UNKNOWN;
  }

  return rc;
}
