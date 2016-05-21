#!/usr/bin/env python3

from math import *

A1 = 160
A2 = 173

GEAR_RATIO = 5
MICRO_STEPS = 2
STEPS_PER_REV = 200
PULSES_PER_REV = MICRO_STEPS * STEPS_PER_REV * GEAR_RATIO
RADIANS_PER_REV = 2 * pi

# The following are used in the inverse kinematics calculation, and are
# constant, so we precalculate them.
SQ_A1_PLUS_SQ_A2 = (A1 * A1) + (A2 * A2)
A1_A2_2 = A1 * A2 * 2

def radians(degrees):
    return degrees * pi / 180

def degrees(radians):
    return radians * 180 / pi

def pulses_to_radians(pulses):
    return pulses * RADIANS_PER_REV / PULSES_PER_REV

def radians_to_pulses(radians):
    return int(radians * PULSES_PER_REV / RADIANS_PER_REV)

def inverse_kinematics(x, y):
    q2 = acos(((x * x) + (y * y) - SQ_A1_PLUS_SQ_A2) / A1_A2_2);
    q1 = atan2(y, x) - atan2(A2 * sin(q2), A1 + A2 * cos(q2));
    return q1, q2

def inverse_kinematics_pulses(x, y):
    q1, q2 = inverse_kinematics(x, y)
    return radians_to_pulses(q1), radians_to_pulses(q2)

def forward_kinematics(q1, q2):
    x = A2 * cos(q1 + q2) + A1 * cos(q1)
    y = A2 * sin(q1 + q2) + A1 * sin(q1)
    return x, y

def forward_kinematics_pulses(p1, p2):
    return forward_kinematics(pulses_to_radians(p1), pulses_to_radians(p2))

def test():
    x, y = forward_kinematics(0, 0)
    q1, q2 = inverse_kinematics(x, y)
    print('{:7.3f} {:7.3f} {:7.3f} {:7.3f}'.format(x, y, q1, q2))

    x, y = forward_kinematics(0, radians(90))
    q1, q2 = inverse_kinematics(x, y)
    print('{:7.3f} {:7.3f} {:7.3f} {:7.3f}'.format(x, y, q1, q2))

    x, y = forward_kinematics(radians(90), 0)
    q1, q2 = inverse_kinematics(x, y)
    print('{:7.3f} {:7.3f} {:7.3f} {:7.3f}'.format(x, y, q1, q2))

class Interpolator:

    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def interpolate(self, t):
        x = self.x1 + (self.x2 - self.x1) * t
        y = self.y1 + (self.y2 - self.y1) * t
        return x, y

    def interpolate_y(self, x):
        t = (x - self.x1) / (self.x2 - self.x1)
        y = self.y1 + (self.y2 - self.y1) * t
        return y

    def len(self):
        return sqrt((self.x2 - self.x1)**2 + (self.y2 - self.y1)**2)

def test2():

    pulses = []
    interp = Interpolator(100, 0, 0, 100)
    distance = interp.len() / 10
    for t in range(int(distance + 1)):
        x, y = interp.interpolate(t/distance)
        p1, p2 = inverse_kinematics_pulses(x, y)
        pulses.append([p1, p2])
        rx, ry = forward_kinematics_pulses(p1, p2)
        print('{:7.3f} {:7.3f} {:7.3f} {:7.3f} {:5d} {:5d}'.format(x, y, rx, ry, p1, p2))

    plt_x = []
    plt_y = []
    last_pulse = pulses[0]
    for pulse in pulses[1:]:
        p1a = last_pulse[0]
        p2a = last_pulse[1]
        p1b = pulse[0]
        p2b = pulse[1]

        p = Interpolator(p1a, p2a, p1b, p2b)
        for p1 in range(p1a, p1b):
            p2 = int(p.interpolate_y(p1))
            x, y = forward_kinematics_pulses(p1, p2)
            plt_x.append(x)
            plt_y.append(y)
        last_pulse = pulse

    import matplotlib.pyplot as plt
    plt.plot(plt_x, plt_y)
    plt.show()

test2()

