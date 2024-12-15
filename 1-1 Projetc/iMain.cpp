# include "iGraphics.h"
//general macros
#define TILE_SIZE 128
#define MAX_TILE_POWER 16
#define SCREEN_WIDTH 572
#define SCREEN_HEIGHT 780

//animation macros
#define ANIMATION_COUNTER 20

//logo coords
#define LOGO_X 180
#define LOGO_Y 480

//StarGame logo placements
#define START_GAME_X 150
#define START_GAME_Y 420
#define START_GAME_HEIGHT 64
#define START_GAME_WIDTH 288

//How To Play placements
#define HOW_TO_PLAY_X 150
#define HOW_TO_PLAY_Y 340
#define HOW_TO_PLAY_HEIGHT 64
#define HOW_TO_PLAY_WIDTH 288

//Leaderboard placement
#define LEADERBOARD_X 150 
#define LEADERBOARD_Y 260
#define LEADERBOARD_HEIGHT 64
#define LEADERBOARD_WIDTH 288
#define LEADERBOARD_TEXT_X 210
#define LEADERBOARD_TEXT_Y 600

//About placements
#define ABOUT_X 240
#define ABOUT_Y 150
#define ABOUT_HEIGHT 96
#define ABOUT_WIDTH 96

//Quit placements
#define QUIT_X 240
#define QUIT_Y 40
#define QUIT_HEIGHT 96
#define QUIT_WIDTH 96

//Game Texts
#define GAME_TEXT_X 420
#define GAME_TEXT_Y 760

//Game button
#define GAME_BUTTON_X 435
#define GAME_BUTTON_Y 745
#define RADIUS 10
#define SLICES 1000

//necessary offsets
#define OFFSET 50
#define OFFSET2 5

//music coords
#define MUSIC_TEXT_X 250
#define MUSIC_TEXT_Y 720

//Moved to text
#define MOVED_TO_TEXT_X 250
#define MOVED_TO_TEXT_Y 600

//score
#define SCORE_COUNT 5

//necessary structs
struct MovingTilesStruct{
	int fromRow,toRow,fromCol,toCol,value;
}movingTiles[16];


//game functions
void showBlocks();
void showBlockHolders();

//sliding functions with animation
void slideUp();
void slideDown();
void slideLeft();
void slideRight();

//initializing game
void generateTile();
void restartGame();
void initialGrid();
bool isGameOver();

//score
char* scoreFunc();

//animation. basically a counter
void animate();

//menu buttons
void image();

//autoplay funcs
long long int calculatePossibleScore(int grid[4][4]);
void slideLeftNoAnimation(int grid[4][4]);
void slideRightNoAnimation(int grid[4][4]);
void slideUpNoAnimation(int grid[4][4]);
void slideRightNoAnimation(int grid[4][4]);
bool areGridsEqual(int temp[4][4],int grid[4][4]);
char optimalSlide();
void autosolver();

//leaderboard func
void storeScores(int score);
void showScores();
//global variables

enum GameStates {MENU,GAME_ON,AUTOPLAY,HOW_TO,LEADERBOARD,ABOUT};
GameStates currentState = MENU;

enum GameOnButton {NO,HELP,RESTART,QUIT};
GameOnButton hoveredButton=NO;

enum MenuButton {NOPE,START,HTP,QU,CREDITS,HIGHSCORES};
MenuButton menuHoverButton=NOPE;

enum SlideStates{N,LEFT,RIGHT,UP,DOWN};
SlideStates slide=N;

const int gridOrigin=32;

//animation
bool isRunning=false;
bool isAnimating = false;

// misc
bool gameStarted=false;
bool autoPlayMode=false;
bool scoreSaved=false;

/// score
long long int score=0;

//music
bool musicOn=true;

int gridPositionX[4]={gridOrigin,gridOrigin+TILE_SIZE,gridOrigin+TILE_SIZE*2,gridOrigin+TILE_SIZE*3};
int gridPositionY[4]={gridOrigin+TILE_SIZE*3,gridOrigin+TILE_SIZE*2,gridOrigin+TILE_SIZE,gridOrigin};

//grid array
int grid[4][4]={
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

char gridBlockNames[12][80];
void populateBlocks(){
	for(int i=0;i<=16;i++){
		if(i<10){
			sprintf(gridBlockNames[i],"GameFiles\\PNGTiles\\00%d.png",i);
		}
		else{
			sprintf(gridBlockNames[i],"GameFiles\\PNGTiles\\0%d.png",i);
		}
		printf("%s\n",gridBlockNames[i]);
	}
}
/*c
	function iDraw() is called again and again by the system.

	*/

void iDraw() {
	//place your drawing codes here
	iClear();
	if(currentState == MENU){
		iShowBMP(0,0,"GameFiles\\Background 2.bmp");
		image(); //menu buttons
	}
	else if(currentState == GAME_ON){
		iShowBMP(0,0,"GameFiles\\Background.bmp");
		iSetColor(0,0,0);
		iText(10,760,scoreFunc(),GLUT_BITMAP_TIMES_ROMAN_24);
		
		showBlockHolders();
		showBlocks();

		iText(GAME_TEXT_X,GAME_TEXT_Y,"Help");
		iText(GAME_TEXT_X+40,GAME_TEXT_Y,"Restart");
		iText(GAME_TEXT_X+100,GAME_TEXT_Y,"Quit");

		if(slide==LEFT) iText(MOVED_TO_TEXT_X,MOVED_TO_TEXT_Y,"Moved to Left");
		else if(slide==RIGHT) iText(MOVED_TO_TEXT_X,MOVED_TO_TEXT_Y,"Moved to Right");
		else if(slide==UP) iText(MOVED_TO_TEXT_X,MOVED_TO_TEXT_Y,"Moved to Up");
		else if(slide==DOWN) iText(MOVED_TO_TEXT_X,MOVED_TO_TEXT_Y,"Moved to Down");
		else if(slide==N) iText(MOVED_TO_TEXT_X,MOVED_TO_TEXT_Y,"Start the game!");
		{
			//for coloring i.e darkening buttons when hovered
			int helpState=(hoveredButton==HELP),restartState=(hoveredButton==RESTART),quitState=(hoveredButton==QUIT);
			iSetColor(55-helpState*OFFSET,255-helpState*OFFSET,55-helpState*OFFSET);iFilledCircle(GAME_BUTTON_X,GAME_BUTTON_Y,RADIUS,SLICES);
			iSetColor(255-restartState*OFFSET,255-restartState*OFFSET,55-restartState*OFFSET);iFilledCircle(GAME_BUTTON_X+50,GAME_BUTTON_Y,RADIUS,SLICES);
			iSetColor(255-quitState*OFFSET,55-quitState*OFFSET,55-quitState*OFFSET);iFilledCircle(GAME_BUTTON_X+100,GAME_BUTTON_Y,RADIUS,SLICES);
		}
		//gameover display
		if(isGameOver()){
			iSetColor(250,0,0);
			iText(245,635,"Game Over!",GLUT_BITMAP_TIMES_ROMAN_24);

			if(!autoPlayMode && score>0){
				if(!scoreSaved){
					storeScores(score);
					scoreSaved=true;
				}
				
			}
		}
		//autoplay display
		char autoPlayString[80];
		sprintf(autoPlayString,"Autoplay: %s",(autoPlayMode?"ON":"OFF"));
		iSetColor(0,0,0);
		iText(MUSIC_TEXT_X-10,MUSIC_TEXT_Y-40,autoPlayString);
		iRectangle(315,677,30,20);
		iText(MUSIC_TEXT_X-85,MUSIC_TEXT_Y-60,"'a' to turn autoplay off or on",GLUT_BITMAP_9_BY_15);
		
	}
	else if(currentState==HOW_TO){
		{
			Image aboutBG;
			iLoadImage(&aboutBG,"GameFiles\\Background 2(Help).png");
			iShowImage(0,0,&aboutBG);
			iFreeImage(&aboutBG);
		}
		if(gameStarted){
			iSetColor(0,0,0);
			iText(GAME_TEXT_X,GAME_TEXT_Y,"Back");
			int helpState=(hoveredButton==HELP);
			iSetColor(55-helpState*OFFSET,255-helpState*OFFSET,55-helpState*OFFSET);
			iFilledCircle(GAME_BUTTON_X,GAME_BUTTON_Y,RADIUS,SLICES);
		}
		else{
			iSetColor(0,0,0);
			iText(GAME_TEXT_X,GAME_TEXT_Y,"Menu");
			int helpState=(hoveredButton==HELP);
			iSetColor(55-helpState*OFFSET,255-helpState*OFFSET,55-helpState*OFFSET);
			iFilledCircle(GAME_BUTTON_X,GAME_BUTTON_Y,RADIUS,SLICES);
		}
	}
	else if(currentState==ABOUT){
		iSetColor(0,0,0);
		// iText(40,700,"Welcome to 2048, an iGraphics project by 2305151",GLUT_BITMAP_TIMES_ROMAN_24);
		// iRectangle(30,690,520,36);
		// iRectangle(30-6,686-3,520+6*2,36+4*3);
		{
			Image aboutBG;
			iLoadImage(&aboutBG,"GameFiles\\Background (About).png");
			iShowImage(0,0,&aboutBG);
			iFreeImage(&aboutBG);
		}
		iSetColor(0,0,0);
		iText(GAME_TEXT_X,GAME_TEXT_Y,"Menu");
		int helpState=(hoveredButton==HELP);
		iSetColor(55-helpState*OFFSET,255-helpState*OFFSET,55-helpState*OFFSET);
		iFilledCircle(GAME_BUTTON_X,GAME_BUTTON_Y,RADIUS,SLICES);
	}
	else if(currentState==LEADERBOARD){
		{
			Image leaderboardBG;
			iLoadImage(&leaderboardBG,"GameFiles\\Background 2.bmp");
			iShowImage(0,0,&leaderboardBG);
			iFreeImage(&leaderboardBG);
			
		}

		iSetColor(0,0,0);
		iText(GAME_TEXT_X,GAME_TEXT_Y,"Menu");
		int helpState=(hoveredButton==HELP);
		iSetColor(55-helpState*OFFSET,255-helpState*OFFSET,55-helpState*OFFSET);
		iFilledCircle(GAME_BUTTON_X,GAME_BUTTON_Y,RADIUS,SLICES);

		showScores();
	}
	
	//music display
	char musicString[80];
	sprintf(musicString,"Music: %s",(musicOn?"ON":"OFF"));
	iSetColor(0,0,0);
	iText(MUSIC_TEXT_X,MUSIC_TEXT_Y,musicString);
	iRectangle(300,717,35,20);
	iText(MUSIC_TEXT_X-80,MUSIC_TEXT_Y-15,"'m' to turn music off or on",GLUT_BITMAP_9_BY_15);
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	// printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		//printf("x = %d, y= %d\n",mx,my);
		if(currentState==MENU){
			if((mx>=START_GAME_X && mx<=START_GAME_X+START_GAME_WIDTH) && (my>=START_GAME_Y && my<=START_GAME_Y+START_GAME_HEIGHT)) {
				currentState=GAME_ON;
				gameStarted=true;
			}
			else if(mx>=HOW_TO_PLAY_X  && mx<=HOW_TO_PLAY_X+HOW_TO_PLAY_WIDTH && my>=HOW_TO_PLAY_Y && my<=HOW_TO_PLAY_Y+HOW_TO_PLAY_HEIGHT) currentState=HOW_TO;
			else if((mx>=ABOUT_X && mx<=ABOUT_X+ABOUT_WIDTH) && (my>=ABOUT_Y && my<=ABOUT_Y+ABOUT_HEIGHT)) currentState=ABOUT;
			else if((mx>=LEADERBOARD_X && mx<=LEADERBOARD_X+LEADERBOARD_WIDTH) && (my>=LEADERBOARD_Y && my<=LEADERBOARD_Y+LEADERBOARD_HEIGHT)) currentState=LEADERBOARD;
			else if ((mx>=QUIT_X && mx<=QUIT_X+QUIT_WIDTH) && (my>=QUIT_Y && my<=QUIT_Y+QUIT_HEIGHT)) exit(0);
		}
		else if(currentState==GAME_ON){
			if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) currentState=HOW_TO;
			else if(mx>=GAME_BUTTON_X+50-RADIUS && mx<=GAME_BUTTON_X+50+RADIUS && my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS) restartGame();		
			else if(mx>=GAME_BUTTON_X+100-RADIUS && mx<=GAME_BUTTON_X+100+RADIUS && my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS) exit(0);
		}
		else if(currentState==HOW_TO){
			if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) {
				if(gameStarted) currentState=GAME_ON;
				else currentState=MENU;
			}
		}
		else if(currentState==ABOUT){
			if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) currentState=MENU;
		}
		else if(currentState==LEADERBOARD){
			if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) currentState=MENU;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
	}
}

void iPassiveMouseMove(int mx,int my){
	// printf("x = %d, y = %d\n",mx,my);
	if(currentState==MENU){
		if((mx>=START_GAME_X && mx<=START_GAME_X+START_GAME_WIDTH) && (my>=START_GAME_Y && my<=START_GAME_Y+START_GAME_HEIGHT)) menuHoverButton=START;
		else if(mx>=HOW_TO_PLAY_X  && mx<=HOW_TO_PLAY_X+HOW_TO_PLAY_WIDTH && my>=HOW_TO_PLAY_Y && my<=HOW_TO_PLAY_Y+HOW_TO_PLAY_HEIGHT) menuHoverButton=HTP;
		else if(mx>=LEADERBOARD_X  && mx<=LEADERBOARD_X+LEADERBOARD_WIDTH && my>=LEADERBOARD_Y && my<=LEADERBOARD_Y+LEADERBOARD_HEIGHT) menuHoverButton=HIGHSCORES;
		else if((mx>=ABOUT_X && mx<=ABOUT_X+ABOUT_WIDTH) && (my>=ABOUT_Y && my<=ABOUT_Y+ABOUT_HEIGHT)) menuHoverButton=CREDITS;
		else if ((mx>=QUIT_X && mx<=QUIT_X+QUIT_WIDTH) && (my>=QUIT_Y && my<=QUIT_Y+QUIT_HEIGHT)) menuHoverButton=QU;
		else menuHoverButton=NOPE;
	}	
	else if(currentState==GAME_ON){
		if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) hoveredButton=HELP; //help
		else if(mx>=GAME_BUTTON_X+50-RADIUS && mx<=GAME_BUTTON_X+50+RADIUS && my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS) hoveredButton=RESTART; //restart
		else if(mx>=GAME_BUTTON_X+100-RADIUS && mx<=GAME_BUTTON_X+100+RADIUS && my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS) hoveredButton=QUIT; //quit
		else hoveredButton=NO;
	}
	else if(currentState==HOW_TO){
		if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) hoveredButton=HELP; //help or menu button, same code is reused
		else hoveredButton=NO;
	}
	else if(currentState==ABOUT){
		if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) hoveredButton=HELP; //help or menu button
		else hoveredButton=NO;
	}
	else if(currentState==LEADERBOARD){
		if((mx>=GAME_BUTTON_X-RADIUS && mx<=GAME_BUTTON_X+RADIUS) && (my>=GAME_BUTTON_Y-RADIUS && my<=GAME_BUTTON_Y+RADIUS)) hoveredButton=HELP; //help or menu button
		else hoveredButton=NO;
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	//place your codes for other keys here
	// if(currentState==MENU) {
    //     if(key=='s'){
    //         currentState=GAME_ON;
    //     } 
	// 	else if(key=='q'){
    //         exit(0);
    //     }
    // } 

	// if(currentState==GAME_ON){
    //     if(key=='q'){
    //         exit(0);
    //     }
	// 	else if (key=='r') {
    //         restartGame();
    //     }
    // }
	//all these been shifted to mouse use
	if(key=='q') exit(0); //escape button, turns off at any case
	
	else if(key=='m'){
		if(!musicOn){
		musicOn=true;
		PlaySound(TEXT("GameFiles\\Music\\music.wav"),NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);
		}
		else{
			musicOn=false;
			PlaySound(0,0,0);
		}
	}
	else if(currentState==GAME_ON && key=='a'){
		if(autoPlayMode){
			autoPlayMode=false;
			iPauseTimer(1);
		}
		else{
			autoPlayMode=true;
			iResumeTimer(1);
		}
	}
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, gridaows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {
	//place your codes for other keys here
	if(key==GLUT_KEY_END){
		exit(0);
	}
	if(currentState==GAME_ON && !autoPlayMode){
		if(key==GLUT_KEY_DOWN){
			
			slideDown();
			
		}
		else if(key==GLUT_KEY_UP){
			
			slideUp();
		}
		else if(key==GLUT_KEY_RIGHT){
			
			slideRight();
		}
		else if(key==GLUT_KEY_LEFT){
			
			slideLeft();
		}
	}
}


int from_row[16];
int to_row[16];
int from_col[16];
int to_col[16];
int idx=0;
int count = 0;


int main() {
	//place your own initialization codes here.
	score=0;
	populateBlocks();
	srand(time(0));
	initialGrid();
	
	//timers
	iSetTimer(1,animate);
	iPauseTimer(0);
	iSetTimer(1250,autosolver);
	iPauseTimer(1);

	if(musicOn){
		PlaySound(TEXT("GameFiles\\Music\\music.wav"),NULL,SND_FILENAME|SND_LOOP|SND_ASYNC); //autostart
	}

	iInitialize(SCREEN_WIDTH,SCREEN_HEIGHT, "2048, an iGraphics game project by 2305151");
	return 0;
}

void animate() {
	count++;
}

void showBlockHolders(){
	for(int rows=0;rows<4;rows++) {
		for (int cols=0;cols<4;cols++) {
			int first=gridPositionX[cols];
			int second=gridPositionY[rows];
			{
				Image tileBG;
				iLoadImage(&tileBG,gridBlockNames[0]);
				iShowImage(first,second,&tileBG);
				iFreeImage(&tileBG);
			}
			//iShowBMP2(first,second,gridBlockNames[0],0); //only showing block holder tiles
		}
	}
}

void showBlocks(){
	if (isAnimating==true and count== 0) {
		//printf("Animation Started\n");
		isAnimating = false;
		iResumeTimer(0);
	}

	else if (count > 0 and count < ANIMATION_COUNTER) {
		//printf("** count = %d, idx = %d\n", count, idx);
		for (int i=0; i<idx; i++) {
			int initial_x=gridPositionX[movingTiles[i].fromCol],initial_y=gridPositionY[movingTiles[i].fromRow];
			int final_x=gridPositionX[movingTiles[i].toCol],final_y=gridPositionY[movingTiles[i].toRow];
			int current_x = (initial_x +(final_x-initial_x)*count/(ANIMATION_COUNTER));
			int current_y = initial_y +(final_y-initial_y)*count/(ANIMATION_COUNTER);
			// printf("%d %d -> %d %d\n",initial_x,initial_y,final_x,final_y);
			// printf("%d %d\n",current_x,current_y);
			//iShowBMP2(current_x,current_y,gridBlockNames[movingTiles[i].value],0);
			{
				Image tile;
				iLoadImage(&tile,gridBlockNames[movingTiles[i].value]);
				iShowImage(current_x,current_y,&tile);
				iFreeImage(&tile);
				
			}
		}
	//printf("\tcholoman\n");
	}
	else if (count>ANIMATION_COUNTER) {
		//printf("Timer Stopped\n");
		isRunning=false;
		count=0;
		iPauseTimer(0);
	}
	else {
		if(!isRunning){
		for (int rows=0;rows<4;rows++) {
			for (int cols=0;cols<4;cols++) {
				int pic=grid[rows][cols];
				int first=gridPositionX[cols];
				int second=gridPositionY[rows];
				//iShowBMP2(first,second,gridBlockNames[pic],0);
				{
					Image tile;
					iLoadImage(&tile,gridBlockNames[pic]);
					iShowImage(first,second,&tile);
					iFreeImage(&tile);
				}
			}
		}
		//printf("\tstatic\n");
		}
	}
	
	//printf("isRunning:%d\n",isRunning);
}


void slideLeft(){
	//slide
	if(isGameOver() || count) return;
	int isSlide=0;
	idx=0;
	isAnimating = false;
	for(int row=0;row<4;row++){
		for(int col=0;col<4;col++){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoCol=col-1;
				while(gotoCol>=0 && !grid[row][gotoCol] ){
					grid[row][gotoCol]=grid[row][gotoCol+1];
					grid[row][gotoCol+1]=0;
					gotoCol--;
					isSlide=1;
				}
				//anim
				if(tileValue && isSlide){ 
					isAnimating = true; 
					isRunning=true;
				}
				movingTiles[idx].fromRow=movingTiles[idx].toRow=row;
				movingTiles[idx].fromCol=col;
				movingTiles[idx].toCol=gotoCol+1;
				movingTiles[idx].value=tileValue;
				idx++;
			}

		}
	}
	//merge
	for (int row=0;row<4;row++) {
        for (int col=0;col<4-1;col++) {
            if (grid[row][col] && grid[row][col]==grid[row][col+1] && grid[row][col]<MAX_TILE_POWER) {
			grid[row][col]++;         
			grid[row][col+1]=0;
			isSlide=1;
			if(!autoPlayMode) score+=(1<<grid[row][col]);    
            }
        }
    }

	//slide again
    for (int row=0;row<4;row++) {
        for (int col=0;col<4;col++) {
            if (grid[row][col]) {
				int tileValue=grid[row][col];
                int gotoCol=col-1;
                while (gotoCol>=0 && !grid[row][gotoCol]) {
                    grid[row][gotoCol]=grid[row][gotoCol+1];
			        grid[row][gotoCol+1]=0;
                    gotoCol--;
                }
				
            }
        }
    }

	if(isSlide){
		generateTile();
		slide=LEFT;
	}
}

void slideRight(){
	if(isGameOver() || count) return;
	int isSlide=0;
	idx=0;
	isAnimating = false;
	//slide
	for(int row=0;row<4;row++){
		for(int col=3;col>=0;col--){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoCol=col+1;
				while(gotoCol<4 && !grid[row][gotoCol]){
					grid[row][gotoCol]=grid[row][gotoCol-1];
					grid[row][gotoCol-1]=0;
					gotoCol++;
					isSlide=1;
					
				}
				//anim
				if(tileValue && isSlide){ 
					isAnimating = true; 
					isRunning=true;
				}
				movingTiles[idx].fromRow=movingTiles[idx].toRow=row;
				movingTiles[idx].fromCol=col;
				movingTiles[idx].toCol=gotoCol-1;
				movingTiles[idx].value=tileValue;
				idx++;
				
			}
		}
	}
	//merge
	for(int row=0;row<4;row++) {
        for(int col=3;col>0;col--) {
            if(grid[row][col] && grid[row][col]==grid[row][col-1] && grid[row][col]<MAX_TILE_POWER){
				grid[row][col]++;         
				grid[row][col-1]=0;
				isSlide=1;
				if(!autoPlayMode) score+= (1<<grid[row][col]);    
            }
        }
    }
	//slide
	for(int row=0;row<4;row++){
		for(int col=2;col>=0;col--){
			if(grid[row][col]){
				int gotoCol=col+1;
				while(gotoCol<4 && !grid[row][gotoCol]){
					grid[row][gotoCol]=grid[row][gotoCol-1];
					grid[row][gotoCol-1]=0;
					gotoCol++;
				}
			}
		}
	}
	if(isSlide){
		generateTile();
		slide=RIGHT;
	}
}

void slideUp(){
	if(isGameOver() || count) return;
	//slide
	int isSlide=0;
	idx=0;
	isAnimating = false;
	for(int col=0;col<4;col++){
		for(int row=0;row<4;row++){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoRow=row-1;
				while(gotoRow>=0 && !grid[gotoRow][col]){
					grid[gotoRow][col]=grid[gotoRow+1][col];
					grid[gotoRow+1][col]=0;
					gotoRow--;
					isSlide=1;
				}
				//anim
				if(tileValue && isSlide){ 
					isAnimating = true; 
					isRunning=true;
				}
				movingTiles[idx].fromCol=movingTiles[idx].toCol=col;
				movingTiles[idx].fromRow=row;
				movingTiles[idx].toRow=gotoRow+1;
				movingTiles[idx].value=tileValue;
				idx++;
			}
		}
	}
	//merge
	for (int col=0;col<4;col++) {
        for (int row=0;row<3;row++) {
            if (grid[row][col] && grid[row][col]==grid[row+1][col] && grid[row][col]<MAX_TILE_POWER) {
                grid[row][col]++;         
                grid[row+1][col]=0; 
				isSlide=1;
				if(!autoPlayMode) score+= (1<<grid[row][col]);    
            }
        }
    }
	//slide
	for(int col=0;col<4;col++){
		for(int row=1;row<4;row++){
			if(grid[row][col]){
				int gotoRow=row-1;
				while(gotoRow>=0 && !grid[gotoRow][col]){
					grid[gotoRow][col]=grid[gotoRow+1][col];
					grid[gotoRow+1][col]=0;
					gotoRow--;
				}
			}
		}
	}
	if(isSlide){
		generateTile();
		slide=UP;
	}
}

void slideDown(){
	if(isGameOver() || count) return;
	//slide
	idx=0;
	int isSlide=0;
	isAnimating=false;

	for(int col=0;col<4;col++){
		for(int row=3;row>=0;row--){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoRow=row+1;
				while(gotoRow<4 && !grid[gotoRow][col] && gotoRow>=1){
					grid[gotoRow][col]=grid[gotoRow-1][col];
					grid[gotoRow-1][col]=0;
					gotoRow++;
					isSlide=1;
				}
				//anim
				if(tileValue && isSlide){ 
					isAnimating = true; 
					isRunning=true;
				}
				movingTiles[idx].fromCol=movingTiles[idx].toCol=col;
				movingTiles[idx].fromRow=row;
				movingTiles[idx].toRow=gotoRow-1;
				movingTiles[idx].value=tileValue;
				idx++;
			}
		}
	}
	//merge
	for(int col=0;col<4;col++) {
        for(int row=3;row>0;row--) { 
            if (grid[row][col] && grid[row][col] == grid[row-1][col] && grid[row][col]<MAX_TILE_POWER) {
                grid[row][col]++;         
                grid[row-1][col]=0;
				isSlide=1;
				if(!autoPlayMode) score+= (1<<grid[row][col]);    
            }
        }
	}
    //slide
	for(int col=0;col<4;col++){
		for(int row=2;row>=0;row--){
			if(grid[row][col]){
				int gotoRow=row+1;
				while(gotoRow<4 && !grid[gotoRow][col]){
					grid[gotoRow][col]=grid[gotoRow-1][col];
					grid[gotoRow-1][col]=0;
					gotoRow++;
				}

				
			}
		}
	}
	if(isSlide){
		generateTile();
		slide=DOWN;
	}
}

void generateTile() {
    int emptyCells[16][2],emptyCount=0;
    
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(grid[i][j]==0) {  
                emptyCells[emptyCount][0] = i; 
                emptyCells[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }

    if (emptyCount>0) {
        int randIndex=rand()%emptyCount;  
        int randRow=emptyCells[randIndex][0];
        int randCol=emptyCells[randIndex][1];
        grid[randRow][randCol]=(rand()%100<80)?1:2; //randomly choosing 2^1 = 2 or 2^2 = 4 block and placing on the grid
    }
}


void initialGrid(){
	for(int count=0;count<(rand()%2+1);count++){ //randomly 1 or 2 new tiles
		int randRow=rand()%4;
        int randCol=rand()%4;
		grid[randRow][randCol]=(rand()%100<90?1:2); //initiating in the same way as in the generateTiles() func
	}	
}


void restartGame(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			grid[i][j]=0;
		}
	}
	initialGrid();
	score=0;
	slide=N;
}

bool isGameOver(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(grid[i][j]==0) return false;
		}
	} //checking for empty cellls


	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(grid[i][j]==grid[i][j+1] || grid[j][i] ==grid[j+1][i]){ //checking if any moves is possible
				return false;
			}
		}
	}
	return true;
}

char* scoreFunc(){
	static char scoreInString[sizeof(long long int)+10]; //extra size
	sprintf(scoreInString,"Score:%lld",score);
	return scoreInString;
}


void image(){
		Image logo;
		iLoadImage(&logo,"GameFiles\\logo.png");
		iResizeImage(&logo,logo.width/3,logo.height/3);
		iShowImage(LOGO_X,LOGO_Y,&logo);
		iFreeImage(&logo);
		iUnRotate();

		Image startGameButton;
		iLoadImage(&startGameButton,"GameFiles\\Bottom UI.png");
		iResizeImage(&startGameButton,startGameButton.width/2,startGameButton.height/2);
		iShowImage(START_GAME_X+OFFSET2*(menuHoverButton==START),START_GAME_Y+OFFSET2*(menuHoverButton==START),&startGameButton);
		iSetColor(0,0,0);
		iText(START_GAME_X+90,START_GAME_Y+30,"Start Game",GLUT_BITMAP_TIMES_ROMAN_24);
		iFreeImage(&startGameButton);
		
		Image howToPlayButton;
		iLoadImage(&howToPlayButton,"GameFiles\\Bottom UI.png");
		iResizeImage(&howToPlayButton,howToPlayButton.width/2,howToPlayButton.height/2);
		iShowImage(HOW_TO_PLAY_X+OFFSET2*(menuHoverButton==HTP),HOW_TO_PLAY_Y+OFFSET2*(menuHoverButton==HTP),&howToPlayButton);
		iSetColor(0,0,0);
		iText(HOW_TO_PLAY_X+80,HOW_TO_PLAY_Y+30,"How To Play",GLUT_BITMAP_TIMES_ROMAN_24);
		iFreeImage(&howToPlayButton);

		Image leaderboardButton;
		iLoadImage(&leaderboardButton,"GameFiles\\Bottom UI.png");
		iResizeImage(&leaderboardButton,leaderboardButton.width/2,leaderboardButton.height/2);
		iShowImage(LEADERBOARD_X+OFFSET2*(menuHoverButton==HIGHSCORES),LEADERBOARD_Y+OFFSET2*(menuHoverButton==HIGHSCORES),&leaderboardButton);
		iSetColor(0,0,0);
		iText(LEADERBOARD_X+80,LEADERBOARD_Y+30,"Leaderboard",GLUT_BITMAP_TIMES_ROMAN_24);
		iFreeImage(&leaderboardButton);

		Image aboutButton;
		iLoadImage(&aboutButton,"GameFiles\\Green Button.png");
		iShowImage(ABOUT_X+OFFSET2*(menuHoverButton==CREDITS),ABOUT_Y+OFFSET2*(menuHoverButton==CREDITS),&aboutButton);
		iSetColor(0,0,0);
		iText(ABOUT_X+18,ABOUT_Y+45,"About",GLUT_BITMAP_TIMES_ROMAN_24);
		iFreeImage(&aboutButton);

		Image quitButton;
		iLoadImage(&quitButton,"GameFiles\\Pink Button.png");
		iShowImage(QUIT_X+OFFSET2*(menuHoverButton==QU),QUIT_Y+OFFSET2*(menuHoverButton==QU),&quitButton);
		iSetColor(0,0,0);
		iText(QUIT_X+25,QUIT_Y+45,"Quit",GLUT_BITMAP_TIMES_ROMAN_24);
		iFreeImage(&quitButton);
}

void slideLeftNoAnimation(int grid[4][4]){
	for(int row=0;row<4;row++){
		for(int col=0;col<4;col++){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoCol=col-1;
				while(gotoCol>=0 && !grid[row][gotoCol] ){
					grid[row][gotoCol]=grid[row][gotoCol+1];
					grid[row][gotoCol+1]=0;
					gotoCol--;
				}
			}

		}
	}
	//merge
	for (int row=0;row<4;row++) {
        for (int col=0;col<4-1;col++) {
            if (grid[row][col] && grid[row][col]==grid[row][col+1] && grid[row][col]<MAX_TILE_POWER) {
			grid[row][col]++;         
			grid[row][col+1]=0;    
            }
        }
    }

	//slide again
    for (int row=0;row<4;row++) {
        for (int col=0;col<4;col++) {
            if (grid[row][col]) {
				int tileValue=grid[row][col];
                int gotoCol=col-1;
                while (gotoCol>=0 && !grid[row][gotoCol]) {
                    grid[row][gotoCol]=grid[row][gotoCol+1];
            
			        grid[row][gotoCol+1]=0;
                    gotoCol--;
                }
				
            }
        }
    }
}

void slideRightNoAnimation(int grid[4][4]){
	//slide
	for(int row=0;row<4;row++){
		for(int col=3;col>=0;col--){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoCol=col+1;
				while(gotoCol<4 && !grid[row][gotoCol]){
					grid[row][gotoCol]=grid[row][gotoCol-1];
					grid[row][gotoCol-1]=0;
					gotoCol++;
					
				}
				
			}
		}
	}
	//merge
	for(int row=0;row<4;row++) {
        for(int col=3;col>0;col--) {
            if(grid[row][col] && grid[row][col]==grid[row][col-1] && grid[row][col]<MAX_TILE_POWER){
				grid[row][col]++;         
				grid[row][col-1]=0;   
            }
        }
    }
	//slide
	for(int row=0;row<4;row++){
		for(int col=2;col>=0;col--){
			if(grid[row][col]){
				int gotoCol=col+1;
				while(gotoCol<4 && !grid[row][gotoCol]){
					grid[row][gotoCol]=grid[row][gotoCol-1];
					grid[row][gotoCol-1]=0;
					gotoCol++;
				}
			}
		}
	}
}

void slideUpNoAnimation(int grid[4][4]){
	//slide
	for(int col=0;col<4;col++){
		for(int row=0;row<4;row++){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoRow=row-1;
				while(gotoRow>=0 && !grid[gotoRow][col]){
					grid[gotoRow][col]=grid[gotoRow+1][col];
					grid[gotoRow+1][col]=0;
					gotoRow--;
				}
			}
		}
	}
	//merge
	for (int col=0;col<4;col++) {
        for (int row=0;row<3;row++) {
            if (grid[row][col] && grid[row][col]==grid[row+1][col] && grid[row][col]<MAX_TILE_POWER) {
                grid[row][col]++;         
                grid[row+1][col]=0;    
            }
        }
    }
	//slide
	for(int col=0;col<4;col++){
		for(int row=1;row<4;row++){
			if(grid[row][col]){
				int gotoRow=row-1;
				while(gotoRow>=0 && !grid[gotoRow][col]){
					grid[gotoRow][col]=grid[gotoRow+1][col];
					grid[gotoRow+1][col]=0;
					gotoRow--;
				}
			}
		}
	}
}

void slideDownNoAnimation(int grid[4][4]){
	//slide
	for(int col=0;col<4;col++){
		for(int row=3;row>=0;row--){
			if(grid[row][col]){
				int tileValue=grid[row][col];
				int gotoRow=row+1;
				while(gotoRow<4 && !grid[gotoRow][col] && gotoRow>=1){
					grid[gotoRow][col]=grid[gotoRow-1][col];
					grid[gotoRow-1][col]=0;
					gotoRow++;
				}
			}
		}
	}
	//merge
	for(int col=0;col<4;col++) {
        for(int row=3;row>0;row--) { 
            if (grid[row][col] && grid[row][col] == grid[row-1][col] && grid[row][col]<MAX_TILE_POWER) {
                grid[row][col]++;         
                grid[row-1][col]=0;
            }
        }
	}
    //slide
	for(int col=0;col<4;col++){
		for(int row=2;row>=0;row--){
			if(grid[row][col]){
				int gotoRow=row+1;
				while(gotoRow<4 && !grid[gotoRow][col]){
					grid[gotoRow][col]=grid[gotoRow-1][col];
					grid[gotoRow-1][col]=0;
					gotoRow++;
				}

				
			}
		}
	}
}

long long int calculatePossibleScore(int grid[4][4]){
	long long int score,empty;
	score=empty=0;

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(grid[i][j]) score+= (1<<grid[i][j])*(1<<grid[i][j]);
			else empty++;
		}
	}
	return score+empty*2; //returning to judge if the move is worth it
}

char optimalSlide(){
	int highestScore=0;
	char optimal;
	int tempGrid[4][4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			tempGrid[i][j]=grid[i][j];
		}
	}
	//printGrid(tempGrid);
	//initial copy

	//left
	slideLeftNoAnimation(tempGrid);
	int left=calculatePossibleScore(tempGrid);
	//printf("left:%d\n",left);
	//printGrid(tempGrid);
	if(left>highestScore && !areGridsEqual(tempGrid,grid)){
		highestScore=left;
		optimal='l';
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			tempGrid[i][j]=grid[i][j];
		}
	} //restoring again
	

	//right
	slideRightNoAnimation(tempGrid);
	int right=calculatePossibleScore(tempGrid);
	// printf("rt:%d\n",right);
	// printGrid(tempGrid);
	if(right>highestScore && !areGridsEqual(tempGrid,grid)){
		highestScore=right;
		optimal='r';
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			tempGrid[i][j]=grid[i][j];
		}
	}//restoring
	

	//up
	slideUpNoAnimation(tempGrid);
	int up=calculatePossibleScore(tempGrid);
	// printf("up:%d\n",up);
	// printGrid(tempGrid);
	if(up>highestScore && !areGridsEqual(tempGrid,grid)){
		highestScore=up;
		optimal='u';
	}

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			tempGrid[i][j]=grid[i][j];
		}
	}//restoring
	
	//down
	slideDownNoAnimation(tempGrid);
	int down=calculatePossibleScore(tempGrid);
	// printf("dn:%d\n",down);
	// printGrid(tempGrid);
	if(down>highestScore && !areGridsEqual(tempGrid,grid)){
		highestScore=down;
		optimal='d';
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			tempGrid[i][j]=grid[i][j];
		}
	}//restoring
	
	return optimal;
}

void autosolver(){
	//printf("Autoplay:%s\n",autoPlayMode?("YES"):("NO"));
	if(autoPlayMode && !isAnimating && !isGameOver()){
		char optimal = optimalSlide();
		//printf("%c\n",optimal);
		if(optimal=='l') slideLeft();
		else if(optimal=='r') slideRight();
		else if(optimal=='u') slideUp();
		else if(optimal=='d') slideDown();
	}
}

bool areGridsEqual(int temp[4][4],int grid[4][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(temp[i][j]!=grid[i][j]) return false;
		}
	}
	return true;
}
// void printGrid(int grid[4][4]){
// 	for(int i=0;i<4;i++){
// 		for(int j=0;j<4;j++){
// 			printf("%d ",grid[i][j]);
// 		}
// 		printf("\n");
// 	}
// }

void storeScores(int score){
	FILE *fp = fopen("score.txt","a");
	if (fp==NULL){ 
	printf("No");
	return;
	}
	fprintf(fp,"%d\n",score);
	fclose(fp);
	
}

void showScores(){
	FILE *fp = fopen("score.txt","r");
	if (fp==NULL){ 
	printf("No");
	return;
	}

	int n,i,temp,status;
	for(int i=0;1;i++){
		status=fscanf(fp,"%d",&temp);
		if(status==EOF){
			n=i;
			break;
		}
	}
	
	int *scoreArr= (int* )malloc(sizeof(int)*n);
	fseek(fp,0,SEEK_SET);
	for(int i=0;i<n;i++) fscanf(fp,"%d",&scoreArr[i]);
	fclose(fp);

	for(int i=0;i<n-1;i++){
		int maxIdx=i;
		for(int j=i+1;j<n;j++){
			if(scoreArr[j]>scoreArr[maxIdx]){
				maxIdx=j;
			}
		}
		int t=scoreArr[i];
		scoreArr[i]=scoreArr[maxIdx];
		scoreArr[maxIdx]=t;
	}

	// for(int i=0;i<SCORE_COUNT && i<n ;i++){
	// 	//printf("%d-%d\n",i+1,scoreArr[i]);
	// }
	iSetColor(0,0,0);
	iText(LEADERBOARD_TEXT_X,LEADERBOARD_TEXT_Y,"Top five high scores:",GLUT_BITMAP_TIMES_ROMAN_24);
	char scoreTextToShow[80];

	for(int i=0;i<SCORE_COUNT && i<n;i++){
		sprintf(scoreTextToShow,"%d. %d",i+1,scoreArr[i]);
		iText(LEADERBOARD_TEXT_X,LEADERBOARD_TEXT_Y-50*(i+1),scoreTextToShow,GLUT_BITMAP_TIMES_ROMAN_24);
	}
	free(scoreArr);
}
