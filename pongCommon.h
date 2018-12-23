/* LedTable
 *
 * Written by: Ing. David Hrbaty
 * 
 * Common code and defines for the Pong game
 */
 
#define PLAYER_HEIGHT 3
#define MAX_SCORE 5
#define AUTO_PLAYER_SPEED 200

int scorePlayerLeft;
int scorePlayerRight;
  
int positionPlayerLeft;
int positionPlayerRight;
  
int ballx;
int previousBallx;
int bally;
int previousBally;
int velocityx;
int velocityy;
int ballBounces;
  
int gameSpeed;
  
unsigned long lastAutoPlayerMoveTime;
unsigned long rumbleUntil;
unsigned long waitUntil;

