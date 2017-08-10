//Contains functions that generate various fields

#include "field.h"
#include "helmholtz.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <unistd.h>

#define PI 3.14159265

// Hardware default
#define supply_voltage 24.0 // volts

// Current to field ratios - FIND EMPERICALLY
#define current_to_field_small 1.0
#define current_to_field_medium 1.0
#define current_to_field_large 1.0

void static_field(float field_magnitude, int axis) {
    //Generate field using one coil set
    // 0 -> x-axis, 1 -> y-axis, 2 -> z-axis

    if (axis == 0) {
        setXcurrent(current_to_field_small*field_magnitude);
    }
    if (axis == 1) {
        setYcurrent(current_to_field_large*field_magnitude);
    }
    if (axis == 2) {
        setZcurrent(current_to_field_medium*field_magnitude);
    }
}

std::vector<float> rotate_vector(std::vector<float> vector, float alpha, float beta) {
    // Rotate a vector by alpha, beta
    // See Helmholtz_rotation_analysis Jupyter Notebook for rotation matrix
    float alpha_rad = alpha*PI / 180.0;
    float beta_rad = beta*PI / 180.0;
    std::vector<float> new_vector(3, 0);
    new_vector[0] = cos(beta_rad)*cos(alpha_rad)*vector[0] - sin(beta_rad)*vector[1] + cos(beta_rad)*sin(alpha_rad)*vector[2];
    new_vector[1] = sin(beta_rad)*cos(alpha_rad)*vector[0] + cos(beta_rad)*vector[1] + sin(beta_rad)*sin(alpha_rad)*vector[2];
    new_vector[2] = -sin(alpha_rad)*vector[0] + 0.0*vector[1] + cos(alpha_rad)*vector[2];

    return new_vector;
}

std::vector<float> xyz_to_alphabeta(std::vector<float> vector) {
    //take a vector <x,y,z> and back out the rotation angles: alpha, beta (in degrees)
    std::vector<float> angles(2); // (alpha, beta)
    std::vector<float> unit_vector(3);
    float magnitude = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    for (int i = 0; i < vector.size(); ++i) {
        unit_vector[i] = vector[i] / magnitude;
    }
    angles[0] = acos(unit_vector[2])*180.0 / PI; // alpha

    if (vector[0] == 0.0 && vector[1] == 0.0) {
        angles[1] = 0.0; // beta
        return angles;
    }
    else {
        angles[1] = acos(unit_vector[0] / sin(angles[0] * PI / 180.0))*180.0 / PI;
        return angles;
    }
}

void static_field_alphabeta(float field_magnitude, float alpha, float beta) {
    // Creates a field in the direction created by rotating the initial vector: <0,0,1> by alpha (about the original y-axis) then beta (about the original z-axis)
    std::vector<float> initial_vector(3);
    initial_vector[0] = 0.0;
    initial_vector[1] = 0.0;
    initial_vector[2] = 1.0;

    std::vector<float> vector = rotate_vector(initial_vector, alpha, beta);
    float current_x = current_to_field_small*field_magnitude*vector[0]; // assumes that the small coil is the x-axis
    float current_y = current_to_field_large*field_magnitude*vector[1]; // assumes that the large coil is the y-axis
    float current_z = current_to_field_medium*field_magnitude*vector[2]; // assumes that the medium coil is the z-axis

    setXcurrent(current_x);
    setYcurrent(current_y);
    setZcurrent(current_z);

    std::cout << "Current: [" << current_x << ", " << current_y << ", " << current_z << "]\n";

}

bool check_frequency_response(float field_magnitude, float frequency) {
    // See if hardware can produce desired field for a given frequency and field magnitude

    // Values come from: 'Velocity Control with Gravity Compensation for Magnetic Helical Microswimmers' Mahoney
    std::vector<float> R(3);
    R[0] = 0.5; // Ohms, small coil resistance
    R[1] = 0.9; // Ohms, medium coil resistance
    R[2] = 1.6; // Ohms, large coil resistance

    std::vector<float> L(3);
    L[0] = 0.944 / 1000.0; // Henry, small coil inductance
    L[1] = 3.78 / 1000.0; // Henry, medium coil inductance
    L[2] = 12.2 / 1000.0; // Henry, large coil inductance

    std::vector<float> current_to_field_ratio(3);
    current_to_field_ratio[0] = current_to_field_small; // small coil
    current_to_field_ratio[1] = current_to_field_medium; // medium coil
    current_to_field_ratio[2] = current_to_field_large; // large coil

    float V = 0.0;
    for (int i = 0; i < 3; ++i) {
        V = sqrt(R[i] * R[i] + (L[i] * 2 * PI*frequency)*(L[i] * 2 * PI*frequency))*field_magnitude*current_to_field_ratio[i];
        if (V >= supply_voltage) {
            return 0;
        }
    }
    return 1;
}

void rotate_field_alphabeta(float field_magnitude, float alpha, float beta, float frequency, float duration) {
    //Check frequency response
    if (check_frequency_response(field_magnitude, frequency) == 0) {
        std::cout << "ERROR: Hardware capabilities exceeded. Reduce field magnitude or frequency\n";
        return;
    }

    // Rotate a field (mT) with frequency (Hz) for duration (seconds)
    float update = 1.0 / 1000.0; // update currents at 1000Hz
    float theta = 0.0; // degrees
    std::vector<float> rotation_vector(3);
    std::vector<float> rotation_vector_rotated(3);

    float t = 0.0;
    while (t < duration) {
        rotation_vector[0] = cos(theta*PI / 180.0);
        rotation_vector[1] = sin(theta*PI / 180.0);
        rotation_vector[2] = 0.0;

        rotation_vector_rotated = rotate_vector(rotation_vector, alpha, beta);

        float current_x = current_to_field_small*field_magnitude*rotation_vector_rotated[0]; //assumes small coil is x-axis
        float current_y = current_to_field_large*field_magnitude*rotation_vector_rotated[1]; //assumes large coil is y-axis
        float current_z = current_to_field_medium*field_magnitude*rotation_vector_rotated[2]; //assumes medium coil is z-axis

        setXcurrent(current_x);
        setYcurrent(current_y);
        setZcurrent(current_z);

        std::cout <<"current [x,y,z]: [" << current_x << ", " << current_y << ", " << current_z << "]\n";

        usleep(update*1000000.0); //LINUX delay

        theta = theta + update*frequency*360.0;
        if (theta > 360.0) {
            theta = theta - 360.0;
        }
        t = t + update;
    }
}
