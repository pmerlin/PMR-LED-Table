/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Snake game common defines/variables
 */

/* ------ Game related ------ */
#define MAXSNAKEPLAYER 4
uint8_t curLength[MAXSNAKEPLAYER]; //Curren length of snake
int8_t xs[MAXSNAKEPLAYER][127]; //Array containing all snake segments,
int8_t ys[MAXSNAKEPLAYER][127]; // max snake length is array length
int8_t dir[MAXSNAKEPLAYER];     //Current Direction of snake
int8_t alive[MAXSNAKEPLAYER];   //Is snake alive
uint8_t score[MAXSNAKEPLAYER];
#define SNAKEWIDTH  1 //Snake width


boolean snakeGameOver;

uint8_t ax = 0;//Apple x position
uint8_t ay = 0;//Apple y position
//uint8_t acolor = BLUE;
