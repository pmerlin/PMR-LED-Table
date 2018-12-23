/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Snake game common defines/variables
 */

/* ------ Game related ------ */
#define NBPLAYER 2
uint8_t curLength[NBPLAYER];//Curren length of snake
int8_t xs[NBPLAYER][127];//Array containing all snake segments,
int8_t ys[NBPLAYER][127];// max snake length is array length
int8_t dir[NBPLAYER];//Current Direction of snake
uint8_t score[NBPLAYER];
#define SNAKEWIDTH  1 //Snake width


boolean snakeGameOver;

uint8_t ax = 0;//Apple x position
uint8_t ay = 0;//Apple y position
//uint8_t acolor = BLUE;
