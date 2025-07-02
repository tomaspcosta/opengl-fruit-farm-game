#   OpenGL Fruit Harvest Game (C/C++ with GLUT)

##   Table of Contents

1.  [Project Description](#1-project-description)  
2.  [Technologies Used](#2-technologies-used)  
3.  [Gameplay Summary](#3-gameplay-summary)  
4.  [Features](#4-features)  
5.  [How to Run](#5-how-to-run)  

---

##   1. Project Description

This C/C++ project is a 2D two-player farming game developed using OpenGL and GLUT. Players compete to plant and harvest fruits while avoiding moving cars that traverse the game field. The goal is to collect as many fruits as possible and deliver them to the player’s house to gain points.

The game includes a main menu, a “how to play” section, a game loop with score tracking and a timer, and a game over screen showing the final result and winner. The project explores real-time interaction, collision detection, basic animation, and dynamic rendering using low-level graphics APIs.

---

##   2. Technologies Used

* C/C++  
* OpenGL  
* GLUT (e.g. FreeGLUT)  
* Basic game loop logic and timing functions

---

##   3. Gameplay Summary

- **Two Players:**
  - Player 1 (Blue): uses `W`, `A`, `S`, `D`
  - Player 2 (Red): uses Arrow Keys

- **Objective:**
  - Plant fruits, wait for them to grow, and harvest them.
  - Return harvested fruits to your house to score points.
  - Avoid moving cars on the road or you'll lose your harvested fruits.

- **Game Duration:**
  - A countdown timer controls the round.
  - At the end, a winner is declared based on score.

---

##   4. Features

* **Dynamic Grid System**  
  Squares change color as fruits grow, and fade over time.

* **Collision Detection**  
  Prevents players from overlapping and checks collisions with cars.

* **Score Tracking & Timer**  
  Real-time score updates and a countdown timer for the game session.

* **Game States**  
  Includes a main menu, how-to-play instructions, active gameplay, and end screen.

* **Visual Feedback**  
  Simple visual effects and transitions based on player actions.

---

##   5. How to Run

1. Make sure you have **OpenGL** and **GLUT** installed (e.g. FreeGLUT).
2. Compile the code with:

   ```bash
   g++ main.cpp -o fruit-game -lglut -lGLU -lGL
