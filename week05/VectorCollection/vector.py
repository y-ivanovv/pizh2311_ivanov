import json
import math

class Vector:
    """
    Класс, представляющий 2D вектор с компонентами x и y.

    Атрибуты:
        x (float): X-компонента вектора.
        y (float): Y-компонента вектора.
    """

    def __init__(self, x, y):
        """
        Инициализирует новый объект Vector.

        Аргументы:
            x (float): X-компонента вектора.
            y (float): Y-компонента вектора.
        """
        self.x = x
        self.y = y

    def __str__(self):
        """
        Возвращает строковое представление объекта Vector.

        Возвращает:
            str: Строку в формате "Vector(x, y)".
        """
        return f"Vector({self.x}, {self.y})"

    def __add__(self, other):
        """
        Складывает два вектора.

        Аргументы:
            other (Vector): Другой вектор для сложения.

        Возвращает:
            Vector: Новый объект Vector, представляющий сумму двух векторов.
        """
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        """
        Вычитает один вектор из другого.

        Аргументы:
            other (Vector): Вектор для вычитания.

        Возвращает:
            Vector: Новый объект Vector, представляющий разность двух векторов.
        """
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar):
        """
        Умножает вектор на скалярное значение.

        Аргументы:
            scalar (float): Скалярное значение для умножения.

        Возвращает:
            Vector: Новый объект Vector, представляющий масштабированный вектор.
        """
        return Vector(self.x * scalar, self.y * scalar)

    def magnitude(self):
        """
        Вычисляет магнитуду (длину) вектора.

        Возвращает:
            float: Магнитуда вектора.
        """
        return math.sqrt(self.x**2 + self.y**2)

    def dot(self, other):
        """
        Вычисляет скалярное произведение двух векторов.

        Аргументы:
            other (Vector): Другой вектор.

        Возвращает:
            float: Скалярное произведение двух векторов.
        """
        return self.x * other.x + self.y * other.y

    @classmethod
    def from_string(cls, str_value):
        """
        Создает объект Vector из строкового представления.

        Аргументы:
            str_value (str): Строка в формате "x,y".

        Возвращает:
            Vector: Новый объект Vector с компонентами x и y, полученными из строки.
        """
        x, y = map(float, str_value.split(','))
        return cls(x, y)

    def save(self, filename):
        """
        Сохраняет объект Vector в JSON файл.

        Аргументы:
            filename (str): Имя файла для сохранения.
        """
        with open(filename, 'w') as file:
            json.dump({'x': self.x, 'y': self.y}, file)

    @classmethod
    def load(cls, filename):
        """
        Загружает объект Vector из JSON файла.

        Аргументы:
            filename (str): Имя файла для загрузки.

        Возвращает:
            Vector: Новый объект Vector с компонентами x и y, загруженными из файла.
        """
        with open(filename, 'r') as file:
            data = json.load(file)
        return cls(data['x'], data['y'])