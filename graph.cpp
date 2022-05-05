#include <algorithm>
#include "graph.h"

//pos::pos(){}
pos::pos(double x , double y){
    this->x = x;
    this->y = y;
}

pos pos::operator-(const pos &other){
    return pos(x-other.x , y-other.y);
}
pos pos::operator+(const pos &other){
    return pos(x+other.x , y+other.y);
}

pos pos::operator/(const double &other){
    return pos(x/other , y/other);
}
pos pos::operator*(const double &other){
    return pos(x*other , y*other);
}

pos pos::relative_to(pos new_origin){
    return (*this) - new_origin;
}

pos pos::scale(double scale){
    return (*this)*scale;
}

pos pos::unscale(double scale){
    return (*this)/scale;
}

double distance_between(pos begin , pos end){
    return sqrt(pow(end.y-begin.y , 2) + pow(end.x-begin.x , 2));
}

pos mid_point(pos p1 , pos p2){
    return (p1+p2)/2;
}

pos low_pos(pos p1 , pos p2){
    return pos(std::min(p1.x , p2.x) , std::min(p1.y , p2.y));
}

pos high_pos(pos p1 , pos p2){
    return pos(std::max(p1.x , p2.x) , std::max(p1.y , p2.y));
}

void arrange_pos(pos p1 , pos p2){
    auto first = low_pos(p1 , p2);
    auto second = high_pos(p1 , p2);

    p1 = first;
    p2 = second;
}
