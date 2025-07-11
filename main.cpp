// g++ main.cpp -lglut -lGL

#include<cmath>
#include<GL/freeglut.h>
#include<iostream>

#include "essentials.h"

const int WIDTH = 1600, HEIGHT = 900;

camera main_camera(0.0f, 0.0f, -100.0f, 250.0f, (float)WIDTH, (float)HEIGHT);

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool on_screen(position2D pos) {
    if (pos.x < 0 or pos.y < 0 or pos.x > WIDTH or pos.y > HEIGHT)
        return false;
    return true;
}

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(10.0f);

    //line(position2D{100, 800}, position2D{800, 100}, colour{255, 0, 0}, 3.0f);
    //line(position2D{800, 100}, position2D{1500, 800}, colour(0, 255, 0), 3.0f);
    //line(position2D{1500, 800}, position2D{100, 800}, colour(0, 0, 255), 3.0f);

    //line(position2D{-100, -100}, position2D{WIDTH+100, HEIGHT+100}, colour(0, 0, 0), 3.0f);

    //line(main_camera.on_screen(ff), main_camera.on_screen(ss), colour(0, 0, 255), 3.0f);

    object obj("suzanne.obj");
    main_camera.draw_object(obj);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("3D Rendering Stuff");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}