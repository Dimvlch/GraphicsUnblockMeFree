//
//  main.cpp
//  GraphicsUnblockMeFree
//
//  Created by Dimitris Vlachakis on 3/2/25.
//

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Drawing area
static float draw_width = 800;
static float draw_height = 800;

// Margin size
const int margin = 100;

// Block dimensions
const int d = 100;
const int num_x = 6;
const int num_y = 6;
float block_width = d;
float block_height = d;

// Number of blocks
const int bQuant = 8;

// Block selection
int blockSelected = 0;

// Class for the block creation
class Block {
public:
    float x; // Block position
    float y;
    float width; // Block size
    float height;

    float r; // Block color
    float g;
    float b;

    bool bIsHorizontal; // True if the block is horizontal; false if vertical
    bool bIsSelected;   // True if the block is selected; false if not

    void Draw();        // Function to draw the block
    bool Move(int dx, int dy); // Function to move the block

    Block();            // Constructor
    ~Block();           // Destructor
};

// Array of blocks
Block blocks[bQuant];

// Function to draw the block
void Block::Draw() {
    if (bIsSelected) {
        // Draw the block with a distinct orange color
        glColor3f(1.0f, 0.5f, 0.0f); // Orange for selected blocks
        glRectf(x, y, x + width, y + height);

        // Add a white outline for the selected block
        glColor3f(1.0f, 1.0f, 1.0f);  // White outline
        glLineWidth(3);              // Thicker line for better visibility
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
        glLineWidth(1); // Reset line width
    } else {
        // Draw the block with its regular color
        glColor3f(r, g, b);
        glRectf(x, y, x + width, y + height);
    }
}

void renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


// Constructor for Block
Block::Block() : x(0), y(0), width(block_width), height(block_height), r(0), g(1), b(0), bIsHorizontal(false), bIsSelected(false) {}

// Destructor for Block
Block::~Block() {}

// Function to check for collision
bool checkCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2) {
    return !(x1 + width1 <= x2 || x1 >= x2 + width2 || y1 + height1 <= y2 || y1 >= y2 + height2);
}

// Function to initialize blocks
void initBlock(int i, float xAxis, float yAxis, float wdth, float hght, float red, float green, float blue) {
    blocks[i].x = margin + xAxis; // Add margin to x position
    blocks[i].y = margin + yAxis; // Add margin to y position
    blocks[i].width = wdth;
    blocks[i].height = hght;
    blocks[i].r = red;
    blocks[i].g = green;
    blocks[i].b = blue;
    blocks[i].bIsHorizontal = wdth > d;
}

// Function to initialize the game grid with default block position
void init() {
    initBlock(0, 0, 400, d, 2 * d, 0, 0, 1);
    initBlock(1, 100, 400, d, 2 * d, 0, 1, 1);
    initBlock(2, 300, 300, d, 3 * d, 1, 1, 0);
    initBlock(3, 400, 300, d, 3 * d, 0, 1, 1);
    initBlock(4, 500, 300, d, 3 * d, 1, 0, 1);
    initBlock(5, 300, 200, 3 * d, d, 0.5, 0.3, 1);
    initBlock(6, 0, 100, 2 * d, d, 1, 0, 0);
    initBlock(7, 0, 300, 2 * d, d, 0, 1, 0);
}

// Function to draw the grid
void drawGrid() {
    // Highlight the right wall of the grid cell at (500, 400)
        glColor3f(0.0f, 1.0f, 0.0f);  // Yellow color for the highlight
        glLineWidth(4); // Thicker line to emphasize the wall

        // Draw the right wall of the cell at (500, 400)
        glBegin(GL_LINES);
        glVertex2f(600 + d, 400);  // Bottom-right corner of the cell
        glVertex2f(600 + d, 400 + d); // Top-right corner of the cell
        glEnd();

        glLineWidth(1); // Reset line width to default
    
    glColor3f(1.0f, 1.0f, 1.0f);  // White grid lines
    for (int i = 0; i < num_x; i++) {
        for (int j = 0; j < num_y; j++) {
            glBegin(GL_LINE_LOOP);
            glVertex2f(margin + i * d, margin + j * d);
            glVertex2f(margin + (i + 1) * d, margin + j * d);
            glVertex2f(margin + (i + 1) * d, margin + (j + 1) * d);
            glVertex2f(margin + i * d, margin + (j + 1) * d);
            glEnd();
        }
    }
}

// Function to display the blocks and grid
void display(void) {
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw all blocks
    for (int k = 0; k < bQuant; k++) {
        blocks[k].Draw();
    }

    // Draw the grid
    drawGrid();
    
    // Check if the game is won
        if (blocks[7].x == 500 && blocks[7].y == 400) {
            glColor3f(1.0f, 1.0f, 0.0f); // Yellow text for the win message
            renderText(draw_width / 2 - 80, draw_height / 2, "You Win!");
        }

    // Display debug information
    Block& selectedBlock = blocks[blockSelected];
    std::string debugText = "Selected Block ID: " + std::to_string(blockSelected) +
                            " | Position: (" + std::to_string((int)selectedBlock.x) + ", " + std::to_string((int)selectedBlock.y) + ")" +
                            " | Size: (" + std::to_string((int)selectedBlock.width) + " x " + std::to_string((int)selectedBlock.height) + ")" +
                            " | Orientation: " + (selectedBlock.bIsHorizontal ? "Horizontal" : "Vertical");

    glColor3f(1.0f, 1.0f, 1.0f); // White text
    renderText(10, draw_height - 60, debugText);

    glFlush();
}


void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, draw_width, 0.0, draw_height, -1, 1); // Adjusted for margin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// Function to move the selected block
bool Block::Move(int dx, int dy) {
    float newX = x + dx;
    float newY = y + dy;

    // Define grid boundaries
    float gridLeft = margin;
    float gridRight = margin + num_x * d;
    float gridBottom = margin;
    float gridTop = margin + num_y * d;

    // Check if the block goes outside the grid boundaries
    if (newX < gridLeft || newX + width > gridRight || newY < gridBottom || newY + height > gridTop) {
        return false; // Block would go out of bounds
    }

    // Check for collisions with other blocks
    for (int i = 0; i < bQuant; i++) {
        if (i != blockSelected) {
            if (checkCollision(newX, newY, width, height, blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height)) {
                return false; // Collision detected
            }
        }
    }

    // If no collision and within bounds, move the block
    x = newX;
    y = newY;
    
    // Check if the block with ID 7 reaches the winning position
        if (blockSelected == 7 && x == 500 && y == 400) {
            std::cout << "You Win!" << std::endl; // Log win in the console
            glutPostRedisplay();                 // Trigger redraw to show the win message
        }
    
    return true;
}


// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    Block& selectedBlock = blocks[blockSelected];

    bool moved = false;
    switch (key) {
        case 'w':  // Move up if possible
            if (!selectedBlock.bIsHorizontal) moved = selectedBlock.Move(0, d);
            break;
        case 's':  // Move down if possible
            if (!selectedBlock.bIsHorizontal) moved = selectedBlock.Move(0, -d);
            break;
        case 'a':  // Move left if possible
            if (selectedBlock.bIsHorizontal) moved = selectedBlock.Move(-d, 0);
            break;
        case 'd':  // Move right if possible
            if (selectedBlock.bIsHorizontal) moved = selectedBlock.Move(d, 0);
            break;
        case 'n':  // Select the next block
            blocks[blockSelected].bIsSelected = false;  // Deselect current block
            blockSelected = (blockSelected + 1) % bQuant;
            blocks[blockSelected].bIsSelected = true;   // Select new block
            glutPostRedisplay();  // Trigger redraw to update debug info and outline
            break;
        case 'r':  // Reset game
            init();
            blockSelected = 0;
            break;
        default:
            break;
    }

    if (moved) {
        glutPostRedisplay();  // Redraw screen after move
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Block Puzzle");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
