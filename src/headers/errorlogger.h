#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H


/*Included headers*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <time.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

/*Used to log errors from the SDL library functions, saves reports in errorlog.txt in the source folder*/
void SDLerrorLogger(const char *msg);

/*Used to log standard c++ erors, saves reports in errorlog.txt in the source folder*/
void errorlogger(const char *errormsg);

/*Used to log standard c++ erors, saves reports in errorlog.txt in the source folder*/
void errorlogger(const char *errormsg, const char* details);

/* Used to print to console */
void debug(const char *msg);
/*=============================================*/

#endif