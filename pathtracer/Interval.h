#ifndef INTERVAL_H
#define INTERVAL_H

//#include "common_utility.h"

class interval {
    public:
        double min, max;

        interval() : min(infinity), max(-infinity) {} //default interval is empty

        interval(double min, double max) : min(min), max(max) {}

        double size() const {
            return max - min;
        }

        bool contains(double x) const {
            return min <= x && x <= max;
        }

        bool surrounds(double x) const {
            return min < x && x < max;
        }

        static const interval empty;
        static const interval universe;
    };

    //const interval interval::empty    = interval(+infinity,-infinity);
    //const interval interval::universe = interval(-infinity, +infinity);

#endif