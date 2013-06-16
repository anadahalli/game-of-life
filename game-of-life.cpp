//-------------------------------------------------------------------------
/// \file	 Game Of Life.cpp
/// \author Ashwath Nadahalli
/// \date	 10 May 2012
/// \brief	 Illustration Of the Conway's Game Of Life using OpenGL
/// \v4
///------------------------------------------------------------------------

#include <iostream>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h> 

using namespace std;

int flag=0;
int font[]={GLUT_BITMAP_TIMES_ROMAN_24,GLUT_BITMAP_9_BY_15};
//void myDisplay(void);

//void printText(int x,int y,char String[]);
//------------------------------------------------------------------------ class gridSystem
class gridSystem{
public:
	gridSystem();								//default constructor
	void drawGrid();							//draw gridlines only
	void clearGrid();							//kill all alive squares 
	void fillCell(const int, const int);		//half-way coordinate retrieval
	void setCellState(const int, const int);	//final coordinate analyzer...sets cell value
	bool getCellState(const int, const int);	//passes back alive/dead state of cell
	void getNextGen();							//manipulates stateArray into next generation
	void randomize();
	void running(const bool);
	bool isEvolving();

private:
	bool stateArray[50][50];					//array to store board data for cells
	bool gameState;
};

gridSystem board;								//declaring a global instance of the gridSystem class
//------------------------------------------------------------------- gridSystem()
gridSystem::gridSystem(){
	gameState = false;  
	//set each cell in grid to false(dead)
	for(int i = 50; i < 0; i++){
		for(int j = 0; j < 50; j++){
			stateArray[j][i] = false;
		}
	}
}

//------------------------------------------------------------------------ running()
void gridSystem::running(const bool state){
	this->gameState = state;
}

//------------------------------------------------------------------------ isEvolving()
bool gridSystem::isEvolving(){
	return this->gameState;
}

//------------------------------------------------------------------------ drawGrid()
void gridSystem::drawGrid(){
	//draw gridlines for board cells

		glBegin(GL_LINES);
			glColor3f(0.7,0.5,0.3);
			glVertex2f(600.0,0.0);
			glVertex2f(600.0,900.0);
		glEnd();

	for(float i=50.0; i<=550.0; i+=10.0){
		glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.5);
			glVertex2f(50.0, i);
			glVertex2f(550.0, i);
		glEnd();
	}
	for(float m=50.0; m<=550.0; m+=10.0){
		glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.5);
			glVertex2f(m, 50.0);
			glVertex2f(m, 550.0);
		glEnd();
	}
}

//------------------------------------------------------------------------ clearGrid()
void gridSystem::clearGrid(){
    //clear the grid
	for(int i = 0; i < 50; i++){
		for(int j = 0; j < 50; j++){
			this->stateArray[j][i] = false;
		}
	}
	glutIdleFunc(NULL);  //stop generations if they are going
	glutPostRedisplay();
}

//------------------------------------------------------------------------ setCellState()
void gridSystem::setCellState(const int x, const int y){
	//toggle alive/dead state of specific cell
	int xValue = x / 10 - 5;
	int yValue = y / 10 - 5;
	if(xValue>=0 && xValue<=49 && yValue>=0 && yValue<=49){
		if(this->stateArray[xValue][yValue] == false)
			this->stateArray[xValue][yValue] = true;
		else
			this->stateArray[xValue][yValue] = false;
	}
}

//------------------------------------------------------------------------ getCellState()
bool gridSystem::getCellState(const int x, const int y){
	//returns alive/dead value of cell
	return this->stateArray[x][y];
}

//------------------------------------------------------------------------ fillCell()
void gridSystem::fillCell(const int x, const int y){
	//intermediate state in determining which cell to
	//toggle from mouseclick event
    int vertexY = y/10;
	vertexY*=10;
	int vertexX = x/10;
	vertexX*=10;
	this->setCellState(vertexX,vertexY);      
}

//------------------------------------------------------------------------ getNextGen()
void gridSystem::getNextGen(){
	//create a temporary board
	bool tempGridSystem[50][50] = {false};
	for(int n = 0; n < 50; n++){
		for(int j = 0; j < 50; j++){
			tempGridSystem[j][n] = this->stateArray[j][n];
		}
	}

    for(int i = 0; i < 50; i++) {
		for(int j = 0; j < 50; j++) {
			int neighbors = 0;
			// count the neighbors
			for(int k = -1; k <= 1; k++) {
				int index1 = i + k;
				if(index1 < 0 || index1 >= 50) continue;
				for(int l = -1; l <= 1; l++) {
					int index2 = j + l;
					if(index2 < 0 || index2 >= 50 || (k == 0 && l == 0)) continue;
					if(tempGridSystem[index1][index2]) neighbors++;
				}
			}
			this->stateArray[i][j] = false; // blank out current world
			if(tempGridSystem[i][j]) { // if a cell is occupied
				// has neighbors==2 or 3, the cell survives
				if(neighbors == 2 || neighbors == 3) this->stateArray[i][j] = true;
			}
			else { // if the cell is empty...and
				// if neighbors==3, a cell is born
				if(neighbors == 3) this->stateArray[i][j] = true;
			}
		}
	}
}

//------------------------------------------------------------------------ randomize()
void gridSystem::randomize(){
	//create a random board to play with 
	srand(time(NULL));
	int randomState = 0;
	for(int i = 0; i < 50; i++){
		for(int j = 0; j < 50; j++){
			randomState = 0 + rand() %(1 - 0 + 1);
			if(randomState == 0)
				this->stateArray[j][i] = false;
			else
				this->stateArray[j][i] = true;
		}
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------ myInit()
void myInit()
{
/* set clear color to white and clear window */
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
/* set up standard orthogonal view with clipping */
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluOrtho2D(0.0, 600.0, 0.0, 600.0);

		//glEnable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------------ printText()
void printText(int x,int y,char String[],int f)
{
	glRasterPos2i(x,y);
	int j=strlen(String);
    for(int i=0;i<j;i++)
		glutBitmapCharacter(font[f],String[i]);
}

//-------------------------------------------------------------------- display()
void display(void)
{
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	char Label[] = {"The Game Of Life"};
	char line1[] = {"g -> go"};
	char line2[] = {"s -> stop"};
	char line3[] = {"c -> clear"};
	char line4[] = {"o -> step one"};
	char line5[] = {"h -> help"};
	char line6[] = {"r -> randomize"};
	char state1[] = {"Evolving..."};
	char state2[] = {"Static..."};

	//draw the text

		glColor3f(1.0, 0.5, 0.25);

		glRasterPos2i(60,730);
        for(unsigned k=0; k<strlen(Label); k++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Label[k]);

		glRasterPos2i(300,720);
		for(unsigned m=0; m<strlen(line1); m++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line1[m]);
		glRasterPos2i(300,690);
		for(unsigned n=0; n<strlen(line2); n++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line2[n]);
		glRasterPos2i(300,660);
		for(unsigned o=0; o<strlen(line3); o++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line3[o]);
		glRasterPos2i(420,720);
		for(unsigned p=0; p<strlen(line4); p++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line4[p]);
		glRasterPos2i(420,690);
		for(unsigned q=0; q<strlen(line6); q++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line6[q]);
		glRasterPos2i(420,660);
		for(unsigned r=0; r<strlen(line5); r++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line5[r]);
		
		glColor3f(1.0, 0.0, 0.0);
		glRasterPos2i(460,560);
		if(board.isEvolving()){
			for(unsigned s=0; s<strlen(state1); s++)
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, state1[s]);
		}
		else{
			for(unsigned t=0; t<strlen(state1); t++)
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, state2[t]);
		}

	
	//draw board

	board.drawGrid();

	for(int i = 0; i < 50; i++){
		for(int j = 0; j < 50; j++){
			int vertexY = i * 10 + 50;
			int vertexX = j * 10 + 50;
			if(board.getCellState(j, i) == true){
				glColor3f(0.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
					glVertex2i(vertexX+1, vertexY+1);
					glVertex2i(vertexX+9, vertexY+1);
					glVertex2i(vertexX+9, vertexY+9);
					glVertex2i(vertexX+1, vertexY+9);
				glEnd();
			}
			else{
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
					glVertex2i(vertexX+1, vertexY+1);
					glVertex2i(vertexX+9, vertexY+1);
					glVertex2i(vertexX+9, vertexY+9);
					glVertex2i(vertexX+1, vertexY+9);
				glEnd();
			}
		}
	}
	
	glutSwapBuffers();
	glFlush();
}

//-------------------------------------------------------------------- sleep()
void sleep ( int milli )
{
  clock_t e, s = clock();
  for( e = s + milli; s < e; s = clock() );
}

//-------------------------------------------------------------------- evolving()
void evolving()
{
        board.getNextGen();
		glutPostRedisplay();
		sleep(100);
        //glutPostRedisplay();
}

//-------------------------------------------------------------------- key()
void key(unsigned char k, int xx, int yy)
{
        if (k=='o') {
                board.getNextGen();
                glutPostRedisplay();
        }
        if (k=='g') {
			board.running(true);
			glutIdleFunc(evolving);
            glutPostRedisplay();
        }
        //if (k=='h') 
		if (k=='c') {board.clearGrid(); board.running(false);}
		if (k=='s') {glutIdleFunc(NULL); board.running(false); glutPostRedisplay();}
		if (k=='r') board.randomize();
		
		if (k == 27)
		exit(EXIT_SUCCESS);

		if (k == 13)
		{flag=1;myDisplay();};
}

//-------------------------------------------------------------------- mouse()
void mouse(int button, int state, int x, int y)
{
	int mouse_y = 0; 
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);
    mouse_y = window_height - y;
    // move again
    if (button==GLUT_LEFT_BUTTON  && state == GLUT_DOWN) {
		board.drawGrid();
		board.fillCell(x,mouse_y);
    }
	myDisplay();
}

//-------------------------------------------------------------------- myReshape()
void myReshape(GLsizei w, GLsizei h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,w,h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();    
}
/*void OnReshape(int w, int h)
{
	if (h==0)
		h=1;

	// set the drawable region of the window
	glViewport(0,0,w,h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection
	gluPerspective(45,(float)w/h,0.1,100);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
*/

//------------------------------------------------------------	OnExit()
void OnExit() {
	// make sure we leave game mode when we exit!!
	glutLeaveGameMode();
}

void Intro()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printText(190,530,"\"The Game Of Life\"");
	printText(72,410,"Ashwath Nadahalli");                                            
	printText(180,80,"Press Enter to continue");
 	glutSwapBuffers();
	glFlush();
	
}
void myDisplay(void)
{
	if(flag==0)
		Intro();
	else
		display();
}
//-------------------------------------------------------------------- main()
int main(int argc, char** argv)
{
		// initialise glut
		glutInit(&argc,argv);

		// request a depth buffer, RGBA display mode, and we want double buffering
		glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);
        
		/*
		#ifndef WIN32
		glutGameModeSting("1024x768:32@60");
		#else
		glutGameModeString("width=1024;height=768;bpp=32;");
		#endif
		
		// enter game mode
		glutEnterGameMode();
		*/
		
		glutInitWindowSize(600,800);
        glutInitWindowPosition(500,50);
        glutCreateWindow("The Game Of Life");

		// run our custom initialisation
		myInit();

		// set the function to use to draw our scene
		glutDisplayFunc(myDisplay);

		// set the function to handle changes in screen size
		glutReshapeFunc(myReshape);

		glutIdleFunc(NULL);

		// set the function to handle changes in screen size
		glutKeyboardFunc(key);

		//set the function to handle changes in mouse
		glutMouseFunc (mouse);

		// set the function to be called when we exit
		//atexit(OnExit);

		// this function runs a while loop to keep the program running.
		glutMainLoop();
	
		return 0;
}



