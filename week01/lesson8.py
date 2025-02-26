class Snow:
    def __init__(self, qty):
        self.snow = qty
    def __call__(self, qty):
        self.snow = qty
    def __add__(self, n):
        self.snow += n
    def __sub__(self, n):
        self.snow -= n
    def __mul__(self, n):
        self.snow *= n
    def __truediv__(self, n):
        self.snow = round(self.snow / n)
    def makeSnow(self, row):
        qty_row = int(self.snow / row)
        s = ""
        for i in range(qty_row):
            s += "*" * row + "\n"
        s += "*" * (self.snow - qty_row * row)
        if s[-1] == "\n":
            s = s[:-1]
        return s

snowfall = Snow(23)
print(snowfall.makeSnow(10))

snowfall + 42
print(snowfall.makeSnow(20))

snowfall / 2
print(snowfall.makeSnow(15))

snowfall(100)
print(snowfall.makeSnow(25))