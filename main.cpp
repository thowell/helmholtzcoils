/*
Helmholtz coils (no GUI)

Taylor Howell

10 August 2017

*/


#include "field.h"
#include "helmholtz.h"

#include <iostream>
#include <math.h>
#include <vector>
#include <unistd.h>

int main() {

    turn_on();
    /*
    //1./////////////
    float field_magnitude = 5.0; // mT
    float duration = 20.0; // seconds
    static_field_alphabeta(field_magnitude, 0.0,0.0);
    //usleep(time*1000000.0); // keep the field on for t seconds
    /////////////////
    */

    /*
    //2./////////////
    float field_magnitude = 5.0; // mT
    float duration = 20.0; // seconds
    float alpha = 45.0; // degrees
    float beta = 45.0; // degrees
    static_field_alphabeta(field_magnitude, alpha, beta);
    //usleep(time*1000000.0); // keep the field on for t seconds
    /////////////////
    */

    /*
    //3./////////////
    float field_magnitude = 5.0; // mT
    float duration = 20.0; // seconds
    std::vector<float> vector(3);
    vector[0] = 1.0;
    vector[1] = 1.0;
    vector[2] = 1.0;
    std::vector<float> angles = xyz_to_alphabeta(vector);
    float alpha = angles[0]; // degrees
    float beta = angles[1]; // degrees
    static_field_alphabeta(field_magnitude, alpha, beta);
    //usleep(time*1000000.0); // keep the field on for t seconds
    /////////////////
    */


    //4./////////////
    float field_magnitude = 5.0; // mT
    float duration = 20.0; // seconds
    float alpha = 0.0; // degrees
    float beta = 0.0; // degrees
    float frequency = -5.0; // Hz
    rotate_field_alphabeta(field_magnitude, alpha, beta, frequency, duration);
    /////////////////


    /*
    //5./////////////
    float field_magnitude = 5.0; // mT
    float duration = 2.0; // seconds
    std::vector<float> vector(3);
    vector[0] = 1.0;
    vector[1] = 1.0;
    vector[2] = 1.0;
    std::vector<float> angles = xyz_to_alphabeta(vector);
    float alpha = angles[0]; // degrees
    float beta = angles[1]; // degrees
    float frequency = 5.0; // Hz
    rotate_field_alphabeta(field_magnitude, alpha, beta, frequency, duration);
    /////////////////
    */

    turn_off();
    return 1;
};


