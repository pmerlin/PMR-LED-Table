/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Snake game common defines/variables
 */

/* ------ Game related ------ */
#define MAXNBPLAYER 2
uint8_t curLength[MAXNBPLAYER];//Curren length of snake
int8_t xs[MAXNBPLAYER][127];//Array containing all snake segments,
int8_t ys[MAXNBPLAYER][127];// max snake length is array length
int8_t dir[MAXNBPLAYER];//Current Direction of snake
uint8_t score[MAXNBPLAYER];
#define SNAKEWIDTH  1 //Snake width


boolean snakeGameOver;

uint8_t ax = 0;//Apple x position
uint8_t ay = 0;//Apple y position
//uint8_t acolor = BLUE;
