// g++ main.cpp -lglut -lGL

#include<cmath>
#include<GL/freeglut.h>
#include<iostream>

#include "essentials.h"

const int WIDTH = 1600, HEIGHT = 900;

// standard unit for everything is 1m
// unit for the focal length is mm

camera main_camera(0.0f, 0.0f, -10.0f, 0.05f, (float)WIDTH, (float)HEIGHT);

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool on_display(position2D pos) {
    if (pos.x < 0 or pos.y < 0 or pos.x > WIDTH or pos.y > HEIGHT)
        return false;
    return true;
}

float add = 0.5f;

void movement(unsigned char key, int x, int y) {
    vector3D forward = main_camera.forward();
    float speed = 0.3f;

    if (key == 'w') {
        main_camera.move(forward, speed);
    } else if (key == 's') {
        main_camera.move(forward, -speed);
    } else if (key == 'd') {
        forward.rotate(0.0f, -90.0f, 0.0f);

        main_camera.move(forward, speed);
    } else if (key == 'a') {
        forward.rotate(0.0f, 90.0f, 0.0f);

        main_camera.move(forward, speed);
    } else if (key == 'e') { // up
        forward.rotate(90.0f, 0.0f, 0.0f);

        main_camera.move(forward, speed);
    } else if (key == 'q') { // down
        forward.rotate(-90.0f, 0.0f, 0.0f);

        main_camera.move(forward, speed);
    }
}

void rotation(int key, int x, int y) {
    float speed = 1.0f;

    if (key == 100) { // left arrow key
        main_camera.rotation_y += speed;
    } else if (key == 102) { // right arrow key
        main_camera.rotation_y -= speed;
    } else if (key == 101) { // up arrow key
        main_camera.rotation_x += speed;
    } else if (key == 103) { // down arrow key
        main_camera.rotation_x -= speed;
    }

}
void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glColor3f(0.0f, 0.0f, 0.0f);
    //glPointSize(10.0f);

    //line(position2D{100, 800}, position2D{800, 100}, colour{255, 0, 0}, 3.0f);
    //line(position2D{800, 100}, position2D{1500, 800}, colour(0, 255, 0), 3.0f);
    //line(position2D{1500, 800}, position2D{100, 800}, colour(0, 0, 255), 3.0f);
    //line(position2D{-100, -100}, position2D{WIDTH+100, HEIGHT+100}, colour(0, 0, 0), 3.0f);

    //line(main_camera.on_screen(ff), main_camera.on_screen(ss), colour(0, 0, 255), 3.0f);

    object obj("test_scene.obj");
    main_camera.draw_object(obj, &on_display);
    
    /*
    // cursor
    line(position2D{WIDTH/2-7, HEIGHT/2}, position2D{WIDTH/2+7, HEIGHT/2}, colour{255, 0, 0}, 2.0f);
    line(position2D{WIDTH/2, HEIGHT/2-7}, position2D{WIDTH/2, HEIGHT/2+7}, colour{255, 0, 0}, 2.0f);
    */

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("3D Rendering Stuff");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(movement);
    glutSpecialFunc(rotation);

    glutMainLoop();
    return 0;
}