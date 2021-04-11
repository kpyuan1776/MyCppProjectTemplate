#include "factories.h"


Point::PointFactory::PointFactory(){}

Point Point::PointFactory::NewCartesian(float x, float y)
                {
                        return {x, y};
                }

Point Point::PointFactory::NewPolar(float r, float theta)
                {
                        return {r*cos(theta), r*sin(theta)};
                }


Point::Point(float x, float y): x(x), y(y) {};

ostream &operator<<(ostream& os, const Point& p){
                 os << "x= " << p.x << ",y= " << p.y << endl;
                 return os;
         }