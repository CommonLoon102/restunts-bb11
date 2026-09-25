#ifndef RESTUNTS_SDL3_SCHEDULING_H
#define RESTUNTS_SDL3_SCHEDULING_H

/* Call before SDL initialization so new threads inherit the startup policy. */
void sdl3_configure_process(void);

#endif
