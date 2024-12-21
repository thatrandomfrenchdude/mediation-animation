#!/bin/bash

# Check if Homebrew is installed, if not, provide installation instructions
if ! command -v brew &> /dev/null
then
    echo "Homebrew is not installed. Please install it first:"
    echo "1. Open Terminal"
    echo "2. Run: /bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
    exit 1
fi

# Install GLUT if not already installed
brew list glfw || brew install glfw
brew list glew || brew install glew

# Compile the OpenGL animation
gcc -framework OpenGL -framework GLUT -o x_animation x_animation.c

# Run the animation
./x_animation