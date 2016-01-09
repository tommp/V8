#ifndef TIMER_H
#define TIMER_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Timer{
    private:
        /* The clock time when the timer started */
        Uint32 start_ticks;

        /* The ticks stored when the timer was paused */
        Uint32 paused_ticks;

        /* The timer status */
        bool paused;
        bool started;
    public:
        /* Initializes variables */
        Timer();

        /* The various clock actions */
        void start();
        void restart();
        void stop();
        void pause();
        void unpause();

        /* Gets the timer's time */
        Uint32 get_ticks();

        /* Checks the status of the timer */
        bool is_started(){return started;};
        bool is_paused(){return started && paused;};
};

/*=============================================*/

#endif