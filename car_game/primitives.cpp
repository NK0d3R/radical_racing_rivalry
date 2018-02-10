#include "primitives.h"

void Rect::Clip(const Rect& other) 
{
    int16_t max_x = x + w;
    int16_t max_y = y + h;
    int16_t other_max_x = other.x + other.w;
    int16_t other_max_y = other.y + other.h;

    if(max_x < other.x || x > other_max_x ||
     max_y < other.y || y > other_max_y) {
        w = 0;
        h = 0;
        return;
    }

    if(x < other.x) {
        w -= (other.x - x);
        x = other.x;
        max_x = x + w;
    }

    if(max_x > other_max_x) {
        w -= (max_x - other_max_x);
    }

    if(y < other.y) {
        h -= (other.y - y);
        y = other.y;
        max_y = y + h;
    }

    if(max_y > other_max_y) {
        h -= (max_y - other_max_y);
    }
}

void Rect::ClipPoint(Vector& point) {
    if(point.x < x) {
        point.x = x;
    }

    if(point.x > max_x) {
       point.x = max_x;
    }

    if(point.y < y) {
        point.y = y;
    }

    if(point.y > max_y) {
        point.y = max_y;
    }
}

bool Rect::IsInside(const Vector& point) {
    return point.x >= x && point.x <= max_x &&
           point.y >= y && point.y <= max_y;
}

#define OUTSIDE_LEFT        (1)
#define OUTSIDE_RIGHT       (1<<1)
#define INSIDE              (1<<2)
#define OUTSIDE_MASK        (OUTSIDE_LEFT | OUTSIDE_RIGHT)
#define GET_STATUS_X(xpos)  (xpos < x ? OUTSIDE_LEFT : (xpos > max_x ? OUTSIDE_RIGHT : INSIDE));

void Rect::ClipLineX(Line& line) {
    int8_t startx_status = GET_STATUS_X(line.start.x);
    int8_t endx_status = GET_STATUS_X(line.end.x);
    if(((startx_status | endx_status) & OUTSIDE_MASK) == OUTSIDE_MASK) {
            line.start = {0, 0};
            line.end = {0, 0};
            return;
    }
    Vector diff = line.end - line.start;
    int32_t pitch = TO_FIXED_POINT(diff.y) / diff.x;
    if(startx_status != INSIDE) {
        if(line.start.x < x) {
            line.start.x = x;
        } else {
            line.start.x = max_x;
        }
        line.start.y = line.end.y - FROM_FIXED_POINT(pitch * (line.end.x - line.start.x));
    }
    if(endx_status != INSIDE) {
         if(line.end.x < x) {
            line.end.x = x;
        } else {
            line.end.x = max_x;
        }
        line.end.y = line.start.y + FROM_FIXED_POINT(pitch * (line.end.x - line.start.x));
    }
}

