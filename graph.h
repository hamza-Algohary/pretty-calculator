#ifndef GRAPH_H_
#define GRAPH_H_

#include <math.h>
class pos{
public:
    pos(double x , double y);
    pos() = default;

    double x = 0;
    double y = 0;
    pos operator-(const pos &other);
    pos operator+(const pos &other);
    pos operator/(const double &other);
    pos operator*(const double &other);
    pos relative_to(pos new_origin);
    pos scale(double scale);
    pos unscale(double unscale);
};

pos less_pos(pos p1 , pos p2);
pos high_pos(pos p1 , pos p2);
void arrange_pos(pos p1 , pos p2);
double distance_between(pos begin , pos end);
pos mid_point(pos p1 , pos p2);

#endif