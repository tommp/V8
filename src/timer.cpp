#include "timer.h"

Timer::Timer(){
	start_ticks = 0;
    paused_ticks = 0;

    paused = false;
    started = false;
}

void Timer::start(){
	/* Start the timer */
    started = true;

    /* Unpause the timer */
    paused = false;

    /* Get the current clock time */
    start_ticks = SDL_GetTicks();

    /* Clear tick variables */
    paused_ticks = 0;
}

void Timer::restart(){
    start();
}

void Timer::stop(){
	/* Stop the timer */
    started = false;

    /* Unpause the timer */
    paused = false;

    /* Clear tick variables */
    start_ticks = 0;
    paused_ticks = 0;
}

void Timer::pause(){
	/* If the timer is running and isn't already paused */
    if( started && !paused )
    {
        /* Pause the timer */
        paused = true;

        /* Calculate the paused ticks */
        paused_ticks = SDL_GetTicks() - start_ticks;
        start_ticks = 0;
    }
}

void Timer::unpause(){
	/* If the timer is running and paused */
    if( started && !paused )
    {
        /* Unpause the timer */
        paused = false;

        /* Reset the starting ticks */
        start_ticks = SDL_GetTicks() - paused_ticks;

        /* Reset the paused ticks */
        paused_ticks = 0;
    }
}

Uint32 Timer::get_ticks(){
	/* The actual timer time */
    Uint32 timer_time = 0;

    /* If the timer is running */
    if( started )
    {
        /* If the timer is paused */
        if( paused )
        {
            /* Return the number of ticks when the timer was paused */
            timer_time = paused_ticks;
        }
        else
        {
            /* Return the current time minus the start time */
            timer_time = SDL_GetTicks() - start_ticks;
        }
    }

    return timer_time;
}