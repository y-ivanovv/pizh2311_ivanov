import json
import math
from interfaces import IVector, IVectorSerializable

class Vector(IVector, IVectorSerializable):
    """
    Класс, представляющий 2D вектор с компонентами x и y.
    Реализует интерфейсы IVector и IVectorSerializable.
    """

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return f"Vector({self.x}, {self.y})"

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)

    def magnitude(self):
        return math.sqrt(self.x**2 + self.y**2)

    def dot(self, other):
        return self.x * other.x + self.y * other.y

    @classmethod
    def from_string(cls, str_value):
        x, y = map(float, str_value.split(','))
        return cls(x, y)

    def save(self, filename):
        with open(filename, 'w') as file:
            json.dump({'x': self.x, 'y': self.y}, file)

    @classmethod
    def load(cls, filename):
        with open(filename, 'r') as file:
            data = json.load(file)
        return cls(data['x'], data['y'])