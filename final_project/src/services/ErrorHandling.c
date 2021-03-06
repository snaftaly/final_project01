#include <SDL.h>
#include <SDL_video.h>
#include "ErrorHandling.h"

/* global variables: */
int isError = 0;

/**
 * perrorPrint function:
 * gets a string representing the name of a standard function
 * and uses perror to print that it failed.
 */
void perrorPrint(char * command){
	/* set isError to 1: */
	isError = 1;
	/* create an array of string to hold the message:
	 * the entire message including any of the commands
	 * we use in this program is less than 60 chars */
	char part1[60], part2[60];
	/* put the two parts of the command in two strings */
	strcpy(part1,  "Error: standard function [");
	strcpy(part2, "] has failed.\n");
	/* Concatenate the strings */
	strcat(part1, command);
	strcat(part1, part2);
	/* use perror to print the error */
	perror(part1);
}

/* print an error message to stdErr containing the SDL_GetError() info
 * set isError to 1 */
void sdlErrorPrint(char * sdlProblem){
	/* set isError to 1: */
	isError = 1;
	fprintf(stderr, "ERROR: %s: %s\n", sdlProblem, SDL_GetError());
}

/* print an error message to stdErr and set isError to 1 */
void generalErrorprint(char * errorStr){
	isError = 1;
	fprintf(stderr, "Error: %s\n", errorStr);
}

