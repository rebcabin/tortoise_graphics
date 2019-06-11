#ifndef TORTOISE_GRAPHICS_TORTOISE_H
#define TORTOISE_GRAPHICS_TORTOISE_H

#include <math.h>
#include <libguile.h>

SCM tortoise_hello(void);
SCM tortoise_window();
SCM tortoise_reset ();
SCM tortoise_pendown ();
SCM tortoise_penup ();
SCM tortoise_turn (SCM degrees);
SCM tortoise_move (SCM length);

#endif //TORTOISE_GRAPHICS_TORTOISE_H