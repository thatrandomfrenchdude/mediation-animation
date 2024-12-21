from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

class XShapeAnimation:
    def __init__(self):
        # Animation variables
        self.x_rotation = 0.0
        self.ball_position = 0.0
        self.current_point = 0
        self.animation_time = 0
        self.movement_phase = 0  # 0: outward, 1: inward

        # Coordinates of X points
        self.x_points = [
            [1.0, 1.0, 0.0],    # Top right point
            [-1.0, 1.0, 0.0],   # Top left point
            [-1.0, -1.0, 0.0],  # Bottom left point
            [1.0, -1.0, 0.0]    # Bottom right point
        ]

    def init(self):
        glClearColor(0.0, 0.0, 0.0, 1.0)
        glEnable(GL_DEPTH_TEST)

    def display(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(0.0, 0.0, -5.0)
        
        # Rotate around Z-axis (like a clock face)
        glRotatef(self.x_rotation, 0.0, 0.0, 1.0)
        
        # Draw X shape
        glColor3f(1.0, 1.0, 1.0)
        glBegin(GL_LINES)
        glVertex3f(1.0, 1.0, 0.0)
        glVertex3f(-1.0, -1.0, 0.0)
        glVertex3f(-1.0, 1.0, 0.0)
        glVertex3f(1.0, -1.0, 0.0)
        glEnd()
        
        # Draw ball
        glColor3f(0.0, 1.0, 0.0)  # Green
        glPushMatrix()
        
        # Ball movement logic
        ball_multiplier = (self.ball_position if self.movement_phase == 0 
                           else (1.0 - self.ball_position))
        
        current_x = ball_multiplier * self.x_points[self.current_point][0]
        current_y = ball_multiplier * self.x_points[self.current_point][1]
        current_z = ball_multiplier * self.x_points[self.current_point][2]
        
        glTranslatef(current_x, current_y, current_z)
        glutSolidSphere(0.1, 20, 20)
        glPopMatrix()
        
        glutSwapBuffers()

    def timer(self, value):
        # Rotate clockwise around Z-axis
        self.x_rotation += 1.0
        if self.x_rotation >= 360.0:
            self.x_rotation -= 360.0
        
        # Ball movement
        self.animation_time += 1
        
        if self.animation_time >= 60:  # 2 seconds at 30 fps
            self.animation_time = 0
            self.movement_phase = 1 - self.movement_phase  # Toggle between 0 and 1
            
            if self.movement_phase == 0:
                # Move to next point after completing inward movement
                self.current_point = (self.current_point + 1) % 4
            
            self.ball_position = 0.0
        
        self.ball_position = self.animation_time / 60.0
        
        glutPostRedisplay()
        glutTimerFunc(16, self.timer, 0)  # ~60 fps

    def reshape(self, width, height):
        glViewport(0, 0, width, height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45.0, width / height, 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)

def main():
    glutInit()
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(800, 600)
    glutCreateWindow(b"X Shape Animation")
    
    animation = XShapeAnimation()
    
    animation.init()
    glutDisplayFunc(animation.display)
    glutReshapeFunc(animation.reshape)
    glutTimerFunc(0, animation.timer, 0)
    
    glutMainLoop()

if __name__ == "__main__":
    main()