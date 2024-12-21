// Platform-specific includes: On Mac we use different header files than on other systems
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Animation state variables (similar to class attributes in Python)
float xRotation = 0.0f;      // Tracks rotation angle of the X shape
float ballPosition = 0;       // Position interpolator (0.0 to 1.0)
int currentPoint = 0;         // Which corner of X we're moving to (0-3)
int animationTime = 0;        // Timer counter for animations
int movementPhase = 1;        // 0: ball moving outward, 1: ball moving inward

// Similar to a numpy array - stores 3D coordinates for X corners
float xPoints[4][3] = {
    {1.0f, 1.0f, 0.0f},   // Top right (x, y, z)
    {-1.0f, 1.0f, 0.0f},  // Top left
    {-1.0f, -1.0f, 0.0f}, // Bottom left
    {1.0f, -1.0f, 0.0f}   // Bottom right
};

// Setup function (runs once at start)
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set background color (black)
    glEnable(GL_DEPTH_TEST);                // Enable 3D rendering
}

// Main render function
void display() {
    // Clear screen (both color and depth buffers)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  // Reset transformations
    glTranslatef(0.0f, 0.0f, -5.0f);  // Move scene back so we can see it
    
    // Rotate entire scene
    glRotatef(xRotation, 0.0f, 0.0f, 1.0f);
    
    // Draw X shape using lines
    glColor3f(1.0f, 1.0f, 1.0f);  // White color
    glBegin(GL_LINES);  // Start drawing lines
        // Draw two lines to make X
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    
    // Draw ball (green sphere)
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();  // Save current transformation state
    
    // Calculate ball position (interpolate between center and corner)
    float ballMultiplier = movementPhase == 0 ? ballPosition : (1.0f - ballPosition);
    glTranslatef(
        ballMultiplier * xPoints[currentPoint][0],  // x
        ballMultiplier * xPoints[currentPoint][1],  // y
        ballMultiplier * xPoints[currentPoint][2]   // z
    );
    glutSolidSphere(0.1, 20, 20);  // Draw sphere (radius, segments)
    glPopMatrix();  // Restore transformation state
    
    glutSwapBuffers();
}

// Animation update function (runs every 16ms ≈ 60fps)
void timer(int value) {
    // Update rotation
    xRotation += 1.0f;
    if (xRotation >= 360.0f) xRotation -= 360.0f;
    
    // Update ball animation
    animationTime++;
    
    // Every 60 frames (2 seconds), switch movement direction
    if (animationTime >= 60) {
        animationTime = 0;
        movementPhase = 1 - movementPhase;
        
        if (movementPhase == 0) {
            // Move to next corner when ball returns to center
            currentPoint = (currentPoint + 1) % 4;
        }
        
        ballPosition = 0.0f;
    }
    
    // Calculate smooth movement (0.0 to 1.0)
    ballPosition = (float)animationTime / 60.0f;
    
    glutPostRedisplay();  // Request new frame
    glutTimerFunc(16, timer, 0);  // Schedule next update
}

// Window resize handler
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Main function (program entry point)
int main(int argc, char** argv) {
    // Initialize GLUT (OpenGL Utility Toolkit)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Setup display mode
    glutInitWindowSize(800, 600);  // Set window size
    glutCreateWindow("X Shape Animation");  // Create window with title
    
    // Setup callback functions
    init();
    glutDisplayFunc(display);      // Register display function
    glutReshapeFunc(reshape);      // Register window resize handler
    glutTimerFunc(0, timer, 0);    // Start animation timer
    
    glutMainLoop();  // Start main loop (event processing)
    return 0;
}
