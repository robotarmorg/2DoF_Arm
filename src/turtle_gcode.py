#!/usr/bin/env python3

# https://github.com/robottini/grbl-servo has support for driving an
# RC servo using M03 Sxxx where xxx is 0->255 which maps to 0->180 degrees

from math import sin, cos, pi
import inspect

GCODE_Z_UP = 0
GCODE_Z_DOWN = -1
GCODE_FEED = 1000

class Turtle:

    _pen = None

    def __init__(self):
        self._x = 0
        self._y = 0
        self._gcode_x = 0
        self._gcode_y = 0
        self._gcode_z = 0
        self._feed = 0
        self._heading = 0
        self._isdown = True
        self.degrees()

    def forward(self, distance):
        self._x += distance * cos(self._heading)
        self._y += distance * sin(self._heading)
        self.update()
        
    def backward(self, distance):
        self._x -= distance * cos(self._heading)
        self._y -= distance * sin(self._heading)
        self.update()

    def right(self, angle):
        self._heading -= 2 * pi * angle / self._fullcircle

    def left(self, angle):
        self._heading -= 2 * pi * angle / self._fullcircle

    def goto(self, x, y):
        self._x = x
        self._y = y
        self.update()

    def setx(self, x):
        self._x = x
        self.update()

    def sety(self, y):
        self._y = y
        self.update()

    def setheading(self, to_angle):
        self._heading = 2 * pi * to_angle / self._fullcircle

    def home(self):
        """Move the turtle to the origin."""
        self.goto(0, 0)
        self.setheading(0)

    def position(self):
        return (self._x, self._y)

    def xcor(self):
        return self._x

    def ycor(self):
        return self._y

    def heading(self):
        return self._heading

    def degrees(self, fullcircle=360.0):
        self._fullcircle = fullcircle

    def radians(self):
        self.degrees(2 * pi)

    def pendown(self):
        self._isdown = True

    def penup(self):
        self._isdown = False

    def isdown(self):
        return self._isdown

    def update(self):
        z = GCODE_Z_DOWN if self._isdown else GCODE_Z_UP
        if z != self._gcode_z:
            self.output('G0 Z{}'.format(z))
            self._gcode_z = z
        if self._isdown:
            cmd = 'G1'
        else:
            cmd = 'G0'
        args = ''
        if self._x != self._gcode_x:
            args += 'X{:.1f}'.format(self._x)
            self._gcode_x = self._x
        if self._y != self._gcode_y:
            args += 'Y{:.1f}'.format(self._y)
            self._gcode_y = self._y
        if self._feed == 0:
            args += ' F{}'.format(GCODE_FEED)
            self._feed = GCODE_FEED
        if args:
            self.output('{} {}'.format(cmd, args))

    def output(self, line):
        print(line)

    fd = forward
    bk = backward
    back = backward
    rt = right
    lt = left
    pos = position
    setpos = goto
    setposition = goto
    seth = setheading
    pd = pendown
    down = pendown
    pu = penup
    up = penup

# The following is modified from the original python turtle code

_tg_turtle_functions = [
    'back', 'backward', 'bk', 'degrees', 'down', 'fd',
    'forward', 'goto', 'heading', 'home', 'isdown',
    'left', 'lt', 'pd', 'pendown', 'penup', 'pos', 'position',
    'pu', 'radians', 'right', 'rt', 'seth', 'setheading', 'setpos',
    'setposition', 'setx', 'sety', 'up', 'xcor', 'ycor'
]

def getmethparlist(ob):
    """Get strings describing the arguments for the given object

    Returns a pair of strings representing function parameter lists
    including parenthesis.  The first string is suitable for use in
    function definition and the second is suitable for use in function
    call.  The "self" parameter is not included.
    """
    defText = callText = ""
    # bit of a hack for methods - turn it into a function
    # but we drop the "self" param.
    # Try and build one for Python defined functions
    args, varargs, varkw = inspect.getargs(ob.__code__)
    items2 = args[1:]
    realArgs = args[1:]
    defaults = ob.__defaults__ or []
    defaults = ["=%r" % (value,) for value in defaults]
    defaults = [""] * (len(realArgs)-len(defaults)) + defaults
    items1 = [arg + dflt for arg, dflt in zip(realArgs, defaults)]
    if varargs is not None:
        items1.append("*" + varargs)
        items2.append("*" + varargs)
    if varkw is not None:
        items1.append("**" + varkw)
        items2.append("**" + varkw)
    defText = ", ".join(items1)
    defText = "(%s)" % defText
    callText = ", ".join(items2)
    callText = "(%s)" % callText
    return defText, callText

## The following mechanism makes all methods of RawTurtle and Turtle available
## as functions. So we can enhance, change, add, delete methods to these
## classes and do not need to change anything here.

__func_body = """\
def {name}{paramslist}:
    if {obj} is None:
        {obj} = {init}
    return {obj}.{name}{argslist}
"""

def _make_global_funcs(functions, cls, obj, init):
    for methodname in functions:
        method = getattr(cls, methodname)
        pl1, pl2 = getmethparlist(method)
        if pl1 == "":
            print(">>>>>>", pl1, pl2)
            continue
        defstr = __func_body.format(obj=obj, init=init, name=methodname,
                                    paramslist=pl1, argslist=pl2)
        exec(defstr, globals())

_make_global_funcs(_tg_turtle_functions, Turtle,
                   'Turtle._pen', 'Turtle()')

def demo_pentagram():
    for i in range(5):
        forward(50)
        right(180 - 36)

if __name__ == "__main__":
    demo_pentagram()
