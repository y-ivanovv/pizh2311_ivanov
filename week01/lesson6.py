class Rectangle:
    def __init__(self, width, height):
        self.__width = Rectangle.__test(width)
        self.__height = Rectangle.__test(height)
        print(self)
    def setWidth(self, width):
        self.__width = Rectangle.__test(width)
    def setHeight(self, height):
        self.__height = Rectangle.__test(height)
    def getWidth(self):
        return self.__width
    def getHeight(self):
        return self.__height
    def __test(value):
        if value < 0:
            return abs(value)
        else:
            return value
    def __str__(self):
        return "Rectangle {0}x{1}".format(self.__width, self.__height)
    
a = Rectangle(3, 4)
print(a.getWidth())
a.setWidth(5)
print(a)

b = Rectangle(-2, 4)