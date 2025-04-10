#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sstream>

int gridColors[15][15] = { 0 };
int gridFadeTimes[7][15] = { 0 };
int grid2Colors[15][15] = { 0 };
int squareTimers[7][15] = { 0 };
int squareOwners[15][15];
const int GRID_WIDTH = 7;
const int GRID_HEIGHT = 15;

// PONTOS
typedef struct Pontos {
    float xPonto;
    float yPonto;
} Pontos;

// first player
float character2X = 130.0f;
float character2Y = 130.0f;

bool wKeyPressed = false;
bool aKeyPressed = false;
bool sKeyPressed = false;
bool dKeyPressed = false;

// second player
float characterX = -130.0f;
float characterY = -130.0f;

bool upKeyPressed = false;
bool leftKeyPressed = false;
bool downKeyPressed = false;
bool rightKeyPressed = false;

// scores player1
int player1Score = 0;
int playerFinal1Score = 0;

// scores player1
int player2Score = 0;
int playerFinal2Score = 0;

// carro1
float car1 = 150.0f;

// carro2
float car2 = -150.0f;

// boost
float boost1 = 150.0f;
float boost2 = -150.0f;

// house1
float house1 = -130.0f;
// house2
float house2 = 130.0f;

// timer
int minutes;
int seconds;
bool gameStarted = false;

// menu
bool menuActivated = true;

bool winnerScreenActivated = false;

struct pontos* ptr;
bool howToPlayActivated = false;
int currentPage = 1;

// GameState

enum GameState {
    Menu,

    Game_Over
};
GameState currentState = Menu;

void renderSquare(float x, float y, int color, int fadeTime) {
    if (color == 0) {
        glColor3f(0.7176, 0.7451, 0.4039);
    }
    else if (color == 1) {
        float r, g, b;
        if (fadeTime <= 6000) {
            // Set to green
            r = 0.0;
            g = 1.0;
            b = 0.0;
        }
        else {
            // Set to yellow
            r = 0.0;
            g = 0.5;
            b = 0.0;
        }
        glColor3f(r, g, b);
    }
    else if (color == 3) {
        float r, g, b;
        if (fadeTime <= 6000) {
            // Set to green
            r = 1.0;
            g = 1.0;
            b = 0.0;
        }
        else {
            // Set to yellow
            r = 0.5;
            g = 0.5;
            b = 0.0;
        }
        glColor3f(r, g, b);
    }

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 20, y);
    glVertex2f(x + 20, y - 20);
    glVertex2f(x, y - 20);
    glEnd();

    // Draw black border around square
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + 20, y);
    glVertex2f(x + 20, y - 20);
    glVertex2f(x, y - 20);
    glEnd();
}

void renderGrid() {
    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 15; y++) {
            renderSquare(-150.0f + (x * 20), 150.0f - (y * 20), gridColors[x][y],
                gridFadeTimes[x][y]);
            if ((gridColors[x][y] == 1 || gridColors[x][y] == 3) &&
                gridFadeTimes[x][y] <= 10000) {
                gridFadeTimes[x][y] += 10;
            }
        }
    }

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-110.0, 135.0);
    char scoreString[50];
    sprintf(scoreString, "Player 1 Score: %d", playerFinal1Score);
    for (int i = 0; i < strlen(scoreString); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreString[i]);
    }
}

void renderGrid2() {
    for (int x = 8; x < 15; x++) {
        for (int y = 0; y < 15; y++) {
            renderSquare(-150.0f + (x * 20), 150.0f - (y * 20), gridColors[x][y],
                gridFadeTimes[x][y]);
            if ((gridColors[x][y] == 1 || gridColors[x][y] == 3) &&
                gridFadeTimes[x][y] <= 10000) {
                gridFadeTimes[x][y] += 10;
            }
        }
    }
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(20.0, 135.0);
    char scoreString2[50];
    sprintf(scoreString2, "Player 2 Score: %d", playerFinal2Score);
    for (int i = 0; i < strlen(scoreString2); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreString2[i]);
    }
}

void updateGrid(int characterX, int characterY) {
    int gridX = (int)((characterX + 150) / 20);
    int gridY = (int)((150 - characterY) / 20);
    if ((gridX >= 0 && gridX <= 14) && (gridY >= 0 && gridY <= 14)) {
        // Update the color of the square at (gridX, gridY) to yellow after 6
        // seconds
        if (gridColors[gridX][gridY] != 1 && gridColors[gridX][gridY] != 3) {
            gridColors[gridX][gridY] = 1;
            squareTimers[gridX][gridY] = glutGet(GLUT_ELAPSED_TIME);
            squareOwners[gridX][gridY] = 1;
        }
        else if (gridFadeTimes[gridX][gridY] >= 6000) {
            // The square is in the final color and the player has not passed through
            // it before
            if (squareOwners[gridX][gridY] != 1) {
                gridColors[gridX][gridY] =
                    2;  // Change the square color to the updated final color
                gridFadeTimes[gridX][gridY] = 0;  // Reset the fade time
                player1Score++;                   // Increment player 1 score by 1
            }
            else {
                squareOwners[gridX][gridY] = 0;
            }
        }
    }
}

void updateGrid2(int character2X, int character2Y) {
    int gridX = (int)((character2X + 150) / 20);
    int gridY = (int)((150 - character2Y) / 20);
    if ((gridX >= 0 && gridX <= 14) && (gridY >= 0 && gridY <= 14)) {
        // Update the color of the square at (gridX, gridY) to yellow after 6
        // seconds
        if (gridColors[gridX][gridY] != 1 && gridColors[gridX][gridY] != 3) {
            gridColors[gridX][gridY] = 3;
            squareTimers[gridX][gridY] = glutGet(GLUT_ELAPSED_TIME);
            squareOwners[gridX][gridY] = 2;
        }
        else if (gridFadeTimes[gridX][gridY] >= 6000) {
            // The square is in the final color and the player has not passed through
            // it before
            if (squareOwners[gridX][gridY] != 2) {
                gridColors[gridX][gridY] =
                    2;  // Change the square color to the updated final color
                gridFadeTimes[gridX][gridY] = 0;  // Reset the fade time
                player2Score++;                   // Increment player 2 score by 1
            }
            else {
                squareOwners[gridX][gridY] = 0;
            }
        }
    }
}

void resetGame() {
    characterX = -130.0f;
    characterY = -130.0f;
    character2X = 130.0f;
    character2Y = 130.0f;

    // Reset car positions
    car1 = 150.0f;
    car2 = -150.0f;

    // Reset scores
    player1Score = 0;
    player2Score = 0;
    playerFinal1Score = 0;
    playerFinal2Score = 0;

    // Reset game time
    minutes = 1;
    seconds = 20;
    renderGrid;
    {
        for (int x = 0; x < 7; x++) {
            for (int y = 0; y < 15; y++) {
                renderSquare(-150.0f + (x * 20), 150.0f - (y * 20), gridColors[x][y],
                    gridFadeTimes[x][y]);
                gridColors[x][y] = 0;
                gridFadeTimes[x][y] = 0;
            }
        }

        renderGrid2;
        {
            for (int x = 8; x < 15; x++) {
                for (int y = 0; y < 15; y++) {
                    renderSquare(-150.0f + (x * 20), 150.0f - (y * 20), gridColors[x][y],
                        gridFadeTimes[x][y]);
                    if (gridColors[x][y] = 0) {
                        gridFadeTimes[x][y] = 0;
                    }
                }
            }
        }
    }

    // Clear screeN
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void howToPlay(void) {
    currentPage = 1;
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the text color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Display the title
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(-115, 85);
    char texto1[60] = "HOW TO PLAY";
    for (int i = 0; i < strlen(texto1); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto1[i]);
    }

    // Display the instructions
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-115, 65);
    char texto2[200] =
        "1. Move player 1 using 'W', 'A', 'S', 'D' keys ";
    for (int i = 0; i < strlen(texto2); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto2[i]);
    }

    glRasterPos2f(-115, 45);
    char texto3[200] = "2. Move player 2 using arrow keys (up, down, left, right)";
    for (int i = 0; i < strlen(texto3); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto3[i]);
    }

    glRasterPos2f(-115, 25);
    char texto4[205] =
        "3. To plant a fruit, move a player to a valid planting area";
    for (int i = 0; i < strlen(texto4); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto4[i]);
    }

    glRasterPos2f(-115, 5);
    char texto5[200] =
        "4. Bring harvested crops back to your house to get points [+1 for each crop]";
    for (int i = 0; i < strlen(texto5); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto5[i]);
    }

    glColor3f(1.0, 0.84, 0.0);
    glRasterPos2f(-115, -15);
    char texto8[85] = "GOOD LUCK!";
    for (int i = 0; i < strlen(texto8); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto8[i]);
    }

    glColor3f(1, 1, 1);
    glRasterPos2f(-115, -120);
    char texto9[50] = "Press [ 9 ] to go back to the main menu";
    for (int i = 0; i < strlen(texto9); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto9[i]);
    }

    // edges
    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-150, 150);
    glVertex2f(-140, 150);
    glVertex2f(-140, -150);
    glVertex2f(-150, -150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-140, 150);
    glVertex2f(-140, 140);
    glVertex2f(140, 140);
    glVertex2f(140, 150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-140, -150);
    glVertex2f(-140, -140);
    glVertex2f(140, -140);
    glVertex2f(140, -150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(150, -150);
    glVertex2f(140, -150);
    glVertex2f(140, 150);
    glVertex2f(150, 150);
    glEnd();

    glFlush();
}

void winnerScreen(void) {
    winnerScreenActivated = false;
    gameStarted = false;
    char texto[30] = "GAME OVER";

    char texto4[30] = "[3] END GAME";
    char texto5[30] = "[1] RESTART";

    glColor3f(1.0, 0.0, 0.0);

    glRasterPos2f(-20, 120);
    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }

    glColor3f(1.0, 1.0, 1.0);

    glRasterPos2f(-115, -50);
    for (int i = 0; i < strlen(texto5); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto5[i]);
    }

    glRasterPos2f(35, -50);
    for (int i = 0; i < strlen(texto4); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto4[i]);
    }

    // EDGES
    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-150, 150);
    glVertex2f(-140, 150);
    glVertex2f(-140, -150);
    glVertex2f(-150, -150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-140, 150);
    glVertex2f(-140, 140);
    glVertex2f(140, 140);
    glVertex2f(140, 150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-140, -150);
    glVertex2f(-140, -140);
    glVertex2f(140, -140);
    glVertex2f(140, -150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(150, -150);
    glVertex2f(140, -150);
    glVertex2f(140, 150);
    glVertex2f(150, 150);
    glEnd();

    glFlush();
};

void updateTimer(int value) {
    if (gameStarted) {  // only update the timer if the game has started
        seconds--;

        if (seconds < 0) {
            seconds = 59;
            minutes--;
        }
    }

    // register the timer function again
    glutTimerFunc(1000, updateTimer, 0);
}

void menu(void) {
    char texto1[30] = "OpenGL Project";
    char texto2[30] = "[ 1 ] START GAME";
    char texto3[30] = "[ 2 ] HOW TO PLAY";
    char texto4[30] = "[ 3 ] QUIT GAME";

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-60, 80);
    for (int i = 0; i < strlen(texto1); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto1[i]);
    }

    glRasterPos2f(-115, 45);
    for (int i = 0; i < strlen(texto2); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto2[i]);
    }

    glRasterPos2f(-115, 25);
    for (int i = 0; i < strlen(texto3); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto3[i]);
    }

    glRasterPos2f(-115, 5);
    for (int i = 0; i < strlen(texto4); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto4[i]);
    }

    glRasterPos2f(45, -90);
    for (int i = 0; i < strlen(texto5); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto5[i]);
    }

    glRasterPos2f(45, -100);
    for (int i = 0; i < strlen(texto6); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto6[i]);
    }

    glRasterPos2f(45, -110);
    for (int i = 0; i < strlen(texto7); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto7[i]);
    }

    glRasterPos2f(45, -120);
    for (int i = 0; i < strlen(texto8); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto8[i]);
    }

    glRasterPos2f(45, -130);
    for (int i = 0; i < strlen(texto9); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto9[i]);
    }

    // underline texto1
    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_LINES);
    glVertex2f(-60, 75);
    glVertex2f(57, 75);
    glEnd();

    // edges
    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-150, 150);
    glVertex2f(-140, 150);
    glVertex2f(-140, -150);
    glVertex2f(-150, -150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-140, 150);
    glVertex2f(-140, 140);
    glVertex2f(140, 140);
    glVertex2f(140, 150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(-140, -150);
    glVertex2f(-140, -140);
    glVertex2f(140, -140);
    glVertex2f(140, -150);
    glEnd();

    glColor3f(0.2078, 0.2078, 0.2078);
    glBegin(GL_POLYGON);
    glVertex2f(150, -150);
    glVertex2f(140, -150);
    glVertex2f(140, 150);
    glVertex2f(150, 150);
    glEnd();

    glFlush();
};

void display(void) {
    if (currentPage == 1) {
        if (menuActivated == true) {
            currentPage == 1;
            //----- window size -----
            int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
            int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            float aspectRatio = (float)windowWidth / (float)windowHeight;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            if (aspectRatio > 1.0f) {
                gluOrtho2D(-150.0f * aspectRatio, 150.0f * aspectRatio, -150.0f,
                    150.0f);
            }
            else {
                gluOrtho2D(-150.0f, 150.0f, -150.0f / aspectRatio,
                    150.0f / aspectRatio);
            }
            //---------------------------

            glClearColor(0, 0, 0, 0);
            menu();
        }
    }
    else if (currentPage == 2) {
        if (howToPlayActivated == true) {
            //----- window size -----
            int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
            int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            float aspectRatio = (float)windowWidth / (float)windowHeight;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            if (aspectRatio > 1.0f) {
                gluOrtho2D(-150.0f * aspectRatio, 150.0f * aspectRatio, -150.0f,
                    150.0f);
            }
            else {
                gluOrtho2D(-150.0f, 150.0f, -150.0f / aspectRatio,
                    150.0f / aspectRatio);
            }
            //---------------------------

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            howToPlay();
        }
    }
    if (gameStarted == true) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        //----- window size -----
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float aspectRatio = (float)windowWidth / (float)windowHeight;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (aspectRatio > 1.0f) {
            gluOrtho2D(-150.0f * aspectRatio, 150.0f * aspectRatio, -150.0f, 150.0f);
        }
        else {
            gluOrtho2D(-150.0f, 150.0f, -150.0f / aspectRatio, 150.0f / aspectRatio);
        }
        //---------------------------

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(0.7176, 0.7451, 0.4039);
        glBegin(GL_POLYGON);
        glVertex2f(-150, 150);
        glVertex2f(-10, 150);
        glVertex2f(-10, -150);
        glVertex2f(-150, -150);
        glEnd();

        glColor3f(0.7176, 0.7451, 0.4039);
        glBegin(GL_POLYGON);
        glVertex2f(150, 150);
        glVertex2f(150, -150);
        glVertex2f(10, -150);
        glVertex2f(10, 150);
        glEnd();

        glColor3f(0.2078, 0.1922, 0.1765);
        glBegin(GL_POLYGON);
        glVertex2f(-10, 150);
        glVertex2f(10, 150);
        glVertex2f(10, -150);
        glVertex2f(-10, -150);
        glEnd();

        glColor3f(0.51504, 0.5176, 0.4549);
        glBegin(GL_POLYGON);
        glVertex2f(-10, 150);
        glVertex2f(-12, 150);
        glVertex2f(-12, -150);
        glVertex2f(-10, -150);
        glEnd();

        glColor3f(0.51504, 0.5176, 0.4549);
        glBegin(GL_POLYGON);
        glVertex2f(10, 150);
        glVertex2f(12, 150);
        glVertex2f(12, -150);
        glVertex2f(10, -150);
        glEnd();

        renderGrid();
        renderGrid2();
        updateGrid(characterX, characterY);
        // updateGrid(character2X, character2Y);
        updateGrid2(character2X, character2Y);
        // updateGrid2(characterX, characterY);

        // DRAW ROAD LINES////////////////
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 140);
        glVertex2f(0, 130);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 120);
        glVertex2f(0, 110);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 100);
        glVertex2f(0, 90);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 80);
        glVertex2f(0, 70);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 60);
        glVertex2f(0, 50);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 40);
        glVertex2f(0, 30);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 20);
        glVertex2f(0, 10);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0, -10);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -20);
        glVertex2f(0, -30);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -40);
        glVertex2f(0, -50);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -60);
        glVertex2f(0, -70);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -80);
        glVertex2f(0, -90);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -100);
        glVertex2f(0, -110);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -120);
        glVertex2f(0, -130);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0, -140);
        glVertex2f(0, -150);
        glEnd();
        /////////////////////////////////

        // DRAW 1st character house
        glColor3f(0.5f, 0.35f, 0.05f);
        glPushMatrix();
        glTranslatef(130.0f, 130.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-20.0f, -20.0f);
        glVertex2f(20.0f, -20.0f);
        glVertex2f(20.0f, 20.0f);
        glVertex2f(-20.0f, 20.0f);
        glEnd();
        glPopMatrix();

        // DRAW 2st character house
        glColor3f(0.5f, 0.35f, 0.05f);
        glPushMatrix();
        glTranslatef(-130.0f, -130.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-20.0f, -20.0f);
        glVertex2f(20.0f, -20.0f);
        glVertex2f(20.0f, 20.0f);
        glVertex2f(-20.0f, 20.0f);
        glEnd();
        glPopMatrix();

        // Draw 1st character (direita)
        glColor3f(1.0f, 0.0f, 0.0f);
        glPushMatrix();
        glTranslatef(character2X, character2Y, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(-6.0f, -6.0f);
        glVertex2f(6.0f, -6.0f);
        glVertex2f(6.0f, 6.0f);
        glVertex2f(-6.0f, 6.0f);
        glEnd();
        glPopMatrix();

        // Draw 2nd character (esquerda)
        glColor3f(0.0f, 0.0f, 1.0f);
        glPushMatrix();
        glTranslatef(characterX, characterY, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(-6.0f, -6.0f);
        glVertex2f(6.0f, -6.0f);
        glVertex2f(6.0f, 6.0f);
        glVertex2f(-6.0f, 6.0f);
        glEnd();
        glPopMatrix();

        // Draw object 1 (CAR1 - carro da esquerda)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(-6.20f, car1, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(-3.0f, -6.0f);
        glVertex2f(3.0f, -6.0f);
        glVertex2f(3.0f, 6.0f);
        glVertex2f(-3.0f, 6.0f);
        glEnd();
        glPopMatrix();

        // Draw object 2 (CAR2 - carro da direita)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(6.20f, car2, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(-3.0f, -6.0f);
        glVertex2f(3.0f, -6.0f);
        glVertex2f(3.0f, 6.0f);
        glVertex2f(-3.0f, 6.0f);
        glEnd();
        glPopMatrix();

        // TIMER
        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(-140.0f, 135.0f);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
            '0' + (minutes / 10));  // tens of minutes
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
            '0' + (minutes % 10));  // units of minutes
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ':');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
            '0' + (seconds / 10));  // tens of seconds
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
            '0' + (seconds % 10));  // units of seconds

        // WinnerScreen

        if (minutes == 0 && seconds == 0) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            winnerScreen();
            winnerScreenActivated = true;
            gameStarted = false;
        }

        if (winnerScreenActivated == true) {
            glColor3f(1.0, 1.0, 1.0);

            glRasterPos2f(-115, 80);
            std::string player1Score =
                "PLAYER 1 SCORE = " + std::to_string(playerFinal1Score);
            for (int i = 0; i < player1Score.length(); i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, player1Score[i]);
            }
            glRasterPos2f(-115, 40);
            std::string player2Score =
                "PLAYER 2 SCORE = " + std::to_string(playerFinal2Score);
            for (int i = 0; i < player2Score.length(); i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, player2Score[i]);
            }
            if (playerFinal1Score > playerFinal2Score) {
                glRasterPos2f(-115, 0);
                std::string winnerText = "WINNER: PLAYER 1!";
                for (int i = 0; i < winnerText.length(); i++) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winnerText[i]);
                }
            }
            else if (playerFinal2Score > playerFinal1Score) {
                glRasterPos2f(-115, 0);
                std::string winnerText = "WINNER: PLAYER 2!";
                for (int i = 0; i < winnerText.length(); i++) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winnerText[i]);
                }
            }
            else {
                glRasterPos2f(-115, 0);
                std::string winnerText = "WINNER: IT'S A TIE";
                for (int i = 0; i < winnerText.length(); i++) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winnerText[i]);
                }
            }
        }

        glFlush();
    }
    glutSwapBuffers();
}

void update(int value) {
    // Update 1st character position based on key state
    if (wKeyPressed && characterY < 145.0f) {
        characterY += 1.5f;
    }
    if (aKeyPressed && characterX > -145.0f) {
        characterX -= 1.5f;
    }
    if (sKeyPressed && characterY > -145.0f) {
        characterY -= 1.5f;
    }
    if (dKeyPressed && characterX < 145.0f) {
        characterX += 1.5f;
    }

    // Update 2nd character position based on key state
    if (upKeyPressed && character2Y < 145.0f) {
        character2Y += 1.5f;
    }
    if (leftKeyPressed && character2X > -145.0f) {
        character2X -= 1.5f;
    }
    if (downKeyPressed && character2Y > -145.0f) {
        character2Y -= 1.5f;
    }
    if (rightKeyPressed && character2X < 145.0f) {
        character2X += 1.5f;
    }

    // Check for collisions and block if moving in same direction
    float dx = character2X - characterX;
    float dy = character2Y - characterY;
    float distance = sqrt(dx * dx + dy * dy);
    float radiusSum = 6.0f;
    if (distance < radiusSum) {
        float collisionAngle = atan2(dy, dx);
        float overlap = radiusSum - distance;
        float moveX = overlap * cos(collisionAngle);
        float moveY = overlap * sin(collisionAngle);
        characterX -= moveX / 2.0f;
        characterY -= moveY / 2.0f;
        character2X += moveX / 2.0f;
        character2Y += moveY / 2.0f;

        if ((wKeyPressed && upKeyPressed) || (aKeyPressed && leftKeyPressed) ||
            (sKeyPressed && downKeyPressed) || (dKeyPressed && rightKeyPressed)) {
            characterX += moveX / 2.0f;
            characterY += moveY / 2.0f;
            character2X -= moveX / 2.0f;
            character2Y -= moveY / 2.0f;
        }
    }

    // Check collision between character1 and car1
    if (character2X - 6.0f <= -6.20f + 3.0f &&
        character2X + 6.0f >= -6.20f - 3.0f &&
        character2Y - 6.0f <= car1 + 6.0f && character2Y + 6.0f >= car1 - 6.0f) {
        // Collision detected, reset character1 position
        character2X = 130.0f;
        character2Y = 130.0f;
        car1 = 150.0f;
        player2Score = 0;
    }

    // Check collision between character1 and car2
    if (character2X - 6.0f <= 3.60f + 3.0f &&
        character2X + 6.0f >= 3.60f - 3.0f && character2Y - 6.0f <= car2 + 6.0f &&
        character2Y + 6.0f >= car2 - 6.0f) {
        // Collision detected, reset character1 position
        character2X = 130.0f;
        character2Y = 130.0f;
        car2 = -150.0f;
        player2Score = 0;
    }

    // Check collision between character2 and car1
    if (characterX - 6.0f <= -6.20f + 3.0f &&
        characterX + 6.0f >= -6.20f - 3.0f && characterY - 6.0f <= car1 + 6.0f &&
        characterY + 6.0f >= car1 - 6.0f) {
        // Collision detected, reset character2 position
        characterX = -130.0f;
        characterY = -130.0f;
        car1 = 150.0f;
        player1Score = 0;
    }

    // Check collision between character2 and car2
    if (characterX - 6.0f <= 3.60f + 3.0f && characterX + 6.0f >= 3.60f - 3.0f &&
        characterY - 6.0f <= car2 + 6.0f && characterY + 6.0f >= car2 - 6.0f) {
        // Collision detected, reset character2 position
        characterX = -130.0f;
        characterY = -130.0f;
        car2 = -150.0f;
        player1Score = 0;
    }

    // Player 1 enters house
    if (characterX - 10.0f <= house1 + 10.0f &&
        characterX + 10.0f >= house1 - 10.0f &&
        characterY - 10.0f <= house1 + 10.0f &&
        characterY + 10.0f >= house1 - 10.0f) {
        // Collision detected
        playerFinal1Score += player1Score;
        player1Score = 0;
    }

    // Player 2 enters house
    if (character2X - 10.0f <= house2 + 10.0f &&
        character2X + 10.0f >= house2 - 10.0f &&
        character2Y - 10.0f <= house2 + 10.0f &&
        character2Y + 10.0f >= house2 - 10.0f) {
        // Collision detected
        playerFinal2Score += player2Score;
        player2Score = 0;
    }

    // Update object position
    car1 -= 2.0f;
    car2 += 2.0f;
    boost1 -= 2.0f;
    boost2 += 2.0f;
    // Check if car is out of bounds
    if (car1 < -150.0f) {
        car1 = 150.0f;
    }

    if (car2 > 150.0f) {
        car2 = -150.0f;
    }

    if (110.0 < character2X < 150.0 && 110.0 < character2Y < 150.0) {
        printf("character2X %f\n", character2X);
        printf("character2Y %f\n", character2Y);
    }

    // Request display update
    glutPostRedisplay();

    // Call update again in 16 milliseconds
    glutTimerFunc(16, update, 0);
}

void keyboardDown(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        wKeyPressed = true;
        break;
    case 'a':
        aKeyPressed = true;
        break;
    case 's':
        sKeyPressed = true;
        break;
    case 'd':
        dKeyPressed = true;
        break;

    case '1':
        if (menuActivated == true) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            gameStarted = true;
            menuActivated = false;
            currentState == true;
            resetGame();
        }
        else if (winnerScreenActivated == true) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            menuActivated = true;
            gameStarted = false;
            minutes = 0;
            seconds = 10;
            winnerScreenActivated = false;
            currentPage = 1;
        }
        break;
    case '2':
        if (menuActivated != false) {
            howToPlayActivated = true;
            menuActivated = false;
            gameStarted = false;
            currentPage = 2;
        }
        break;
    case '3':
        if (menuActivated != false) {
            exit(0);
        }
        break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        wKeyPressed = false;
        break;
    case 'a':
        aKeyPressed = false;
        break;
    case 's':
        sKeyPressed = false;
        break;
    case 'd':
        dKeyPressed = false;
        break;
    }

    switch (key) {
    case '1':
        if (winnerScreenActivated == true) {
            if (key == '1') {
                // exit(0);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                menuActivated = true;
                gameStarted = false;
            }
        }
        else {
            gameStarted = true;
            menuActivated = false;
            resetGame();
        }
        break;
    case '2':
        if (menuActivated != false) {
            howToPlayActivated = true;
            menuActivated = false;
            gameStarted = false;
            currentPage = 2;
            break;
            ////// menu how to play
    case '3':
        if (menuActivated != false) {
            exit(0);
            break;
        }
        }
    }
    if (menuActivated == true && winnerScreenActivated == false) {
        if (key == '1') {
            gameStarted = true;
            menuActivated = false;
            currentState = Menu;
            currentPage = 1;
        }
    }
    if (winnerScreenActivated == true || howToPlayActivated == true) {
        if (key == '9') {
            menuActivated = true;
            glClear(GL_COLOR_BUFFER_BIT);
            currentPage = 1;
        }
        if (menuActivated == false && winnerScreenActivated == true) {
            if (key == '3') {
                currentState = Game_Over;
                exit(0);
            }
        }
    }
}

void specialKeyboardDown(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        upKeyPressed = true;
        break;
    case GLUT_KEY_DOWN:
        downKeyPressed = true;
        break;
    case GLUT_KEY_LEFT:
        leftKeyPressed = true;
        break;
    case GLUT_KEY_RIGHT:
        rightKeyPressed = true;
        break;
    }

    switch (key) {
    case '1':
        if (winnerScreenActivated == true) {
            if (key == '1') {
                // exit(0);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                menuActivated = true;
                gameStarted = false;
            }
        }
        else {
            gameStarted = true;
            menuActivated = false;
            resetGame();
        }
        break;
    case '2':
        if (menuActivated != false) {
            howToPlayActivated = true;
            menuActivated = false;
            gameStarted = false;
            currentPage = 2;
            break;
            ////// menu how to play
    case '3':
        if (menuActivated != false) {
            exit(0);
            break;
        }
        }
    }
    if (menuActivated == true && winnerScreenActivated == false) {
        if (key == '1') {
            gameStarted = true;
            menuActivated = false;
            currentState = Menu;
            currentPage = 1;
        }
    }
    if (winnerScreenActivated == true || howToPlayActivated == true) {
        if (key == '9') {
            menuActivated = true;
            glClear(GL_COLOR_BUFFER_BIT);
            currentPage = 1;
        }
        if (menuActivated == false && winnerScreenActivated == true) {
            if (key == '3') {
                currentState = Game_Over;
                exit(0);
            }
        }
    }
}

void specialKeyboardUp(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        upKeyPressed = false;
        break;
    case GLUT_KEY_DOWN:
        downKeyPressed = false;
        break;
    case GLUT_KEY_LEFT:
        leftKeyPressed = false;
        break;
    case GLUT_KEY_RIGHT:
        rightKeyPressed = false;
        break;
    }

    switch (key) {
    case '1':
        if (winnerScreenActivated == true) {
            if (key == '1') {
                // exit(0);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                menuActivated = true;
                gameStarted = false;
                minutes = 0;
                seconds = 10;
                resetGame();
            }
        }
        {
            gameStarted = true;
            menuActivated = false;
        }
        break;
    case '2':
        howToPlayActivated = true;
        menuActivated = false;
        gameStarted = false;
        currentPage = 2;
        break;
    case '3':
        if (menuActivated != false) {
            exit(0);
        }
        break;
    }

    if (menuActivated == true && winnerScreenActivated == false) {
        if (key == '1') {
            gameStarted = true;
            menuActivated = false;
            currentState = Menu;
            currentPage = 1;
        }
    }
    if (winnerScreenActivated == true || howToPlayActivated == true) {
        if (key == '9') {
            menuActivated = true;
            glClear(GL_COLOR_BUFFER_BIT);
            currentPage = 1;
        }
    }
    if (menuActivated == false && winnerScreenActivated == true) {
        if (key == '3') {
            currentState = Game_Over;
            exit(0);
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL-Game");

    glutDisplayFunc(display);
    glutTimerFunc(0, updateTimer, 0);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboardDown);
    glutSpecialUpFunc(specialKeyboardUp);
    resetGame();

    // Start update loop
    glutTimerFunc(1, update, 1);

    glutMainLoop();
    return 0;
}
