#include "girl_math.h"
#include <math.h>

Vector2 Vector2Eerp(Vector2 a, Vector2 b, float t)
{
    Vector2 result = { 0 };

    // No clue...
    result.x = powf(2, (1-t) * log2f(a.x+t) * log2f(b.x));
    result.y = powf(2, (1-t) * log2f(a.y+t) * log2f(b.y));

    return result;
}