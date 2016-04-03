/****************************************************************************
*
*   Copyright (c) 2016 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/

#include "Kinematics.h"

#include <math.h>

#define sq(x)   ((x) * (x))

static const double a1 = 160.0;
static const double a2 = 173.0;
static const double sq_a1_plus_sq_a2_sq = sq(a1) + sq(a2);
static const double _2_a1_a2 = 2.0 * a1 * a2;

// Calculate q1 & q2 (in radians), given x and y (in mm)
void InverseKinematicsQ(double x, double y, double *q1, double *q2)
{
    *q2 = acos((sq(x) + sq(y) - sq_a1_plus_sq_a2_sq) / _2_a1_a2);
    *q1 = atan2(y, x) - atan2(a2 * sin(*q2), a1 + a2 * cos(*q2));
}

// Calculate X & Y (in mm) given q1 and q2 (in radians).
void ForwardKinematicsQ(double q1, double q2, double *x, double *y)
{
    *x = a2 * cos(q1 + q2) + a1 * cos(q1);
    *y = a2 * sin(q1 + q2) + a1 * sin(q1);
}
