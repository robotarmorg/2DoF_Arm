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

#ifndef KINEMATICS_H
#define KINEMATICS_H

extern const double a1;
extern const double a2;

void InverseKinematicsQ(double x, double y, double *q1, double *q2);
void ForwardKinematicsQ(double q1, double q2, double *x, double *y);

#endif  // KINEMATICS_H
