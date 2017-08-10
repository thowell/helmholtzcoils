#ifndef FIELD_H
#define FIELD_H

#include <vector>

void static_field(float field_magnitude, int axis);

std::vector<float> rotate_vector(std::vector<float> vector, float alpha, float beta);

std::vector<float> xyz_to_alphabeta(std::vector<float> vector);

void static_field_alphabeta(float field_magnitude, float alpha, float beta);

bool check_frequency_response(float field_magnitude, float frequency);

void rotate_field_alphabeta(float field_magnitude, float alpha, float beta, float frequency, float duration);

#endif // FIELD_H
