class Rectangle:
    def __init__(self, width, height, sign):
        self.width = int(width)
        self.height = int(height)
        self.sign = str(sign)
    def __str__(self):
        rectangle = []
        for i in range(self.height):
            rectangle.append(self.sign * self.width)
        rectangle = '\n'.join(rectangle)
        return rectangle
    def __add__(self, other):
        return Rectangle(self.width + other.width, self.height + other.height, self.sign)

a = Rectangle(4, 2, 'w')
b = Rectangle(8, 3, 'z')

print(a)
print(b)
print(a + b)
print(b + a)