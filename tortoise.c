#include "tortoise.h"

#include <stdio.h>

static const int WIDTH = 10;
static const int HEIGHT = 10;

void
init_tortoise()
{
    scm_c_define_gsubr("tortoise-hello", 0, 0, 0, tortoise_hello);
}

SCM
tortoise_hello(void) {
    SCM result = scm_from_locale_string("Another turtle made it to the water!\n");
    return result;
}

static FILE *
start_gnuplot ()
{   FILE * result;
    int pipes[2];
    pid_t pid;

    pipe (pipes);
    pid = fork ();

    if (!pid) {
        // This is the "child" branch of the fork that will run the
        // desired sub-process.
        dup2 (pipes[0], STDIN_FILENO);
        execlp ("gnuplot", "gnuplot", (const char *) NULL);
        return NULL; /* Not reached.  */
    }
    // This is the "parent" of the fork that will communicate with
    // the created sub-process.
    result = fdopen (pipes[1], "w");
    return result;
}

static void
draw_line (FILE* output, double x1, double y1, double x2, double y2)
{
    fprintf (output, "plot [0:1] %f + %f * t, %f + %f * t notitle\n",
             x1, x2 - x1, y1, y2 - y1);
    fflush (output);
}

static FILE * g_gnuplot;

static double x, y;
static double direction;
static int    pendown;

SCM
tortoise_window()
{
    fprintf(g_gnuplot, "set multiplot\n");
    fprintf(g_gnuplot, "set parametric\n");
    fprintf(g_gnuplot, "set xrange [-%d:%d]\n", WIDTH, WIDTH);
    fprintf(g_gnuplot, "set yrange [-%d:%d]\n", HEIGHT, HEIGHT);
    fprintf(g_gnuplot, "set size ratio -1\n");
    fprintf(g_gnuplot, "unset xtics\n");
    fprintf(g_gnuplot, "unset ytics\n");
    fflush(g_gnuplot);
    return SCM_UNSPECIFIED;
}

SCM
tortoise_reset ()
{
    x = y = 0.0;
    direction = 0.0;
    pendown = 1;

    fprintf (g_gnuplot, "clear\n");
    fflush (g_gnuplot);

    return SCM_UNSPECIFIED;
}

SCM
tortoise_pendown ()
{
    SCM result = scm_from_bool (pendown);
    pendown = 1;
    return result;
}

SCM
tortoise_penup ()
{
    SCM result = scm_from_bool (pendown);
    pendown = 0;
    return result;
}

SCM
tortoise_turn (SCM degrees)
{
    const double value = scm_to_double (degrees);
    direction += M_PI / 180.0 * value;
    return scm_from_double (direction * 180.0 / M_PI);
}

SCM
tortoise_move (SCM length)
{
    const double value = scm_to_double (length);
    double newX, newY;

    newX = x + value * cos (direction);
    newY = y + value * sin (direction);

    if (pendown) {
        draw_line(g_gnuplot, x, y, newX, newY);
    }
    x = newX;
    y = newY;

    return scm_list_2 (scm_from_double (x), scm_from_double (y));
}
