#!/bin/bash

# check that glfw and glew are installed, exit if not
if ! brew list glfw &> /dev/null || ! brew list glew &> /dev/null
then
    echo "GLFW and GLEW are not installed. Please install them first:"
    echo "1. Open Terminal"
    echo "2. Run: setup.sh"
    exit 1
fi

# Compile the OpenGL animation
gcc -framework OpenGL -framework GLUT -o x_animation x_animation.c

# Run the animation
./x_animation