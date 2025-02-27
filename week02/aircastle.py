from abc import ABC, abstractmethod

class BaseCastle(ABC):
    """Абстрактный базовый класс для всех замков."""

    def __init__(self, height: int, color: str):
        self._height = max(0, height)
        self._color = color

    @abstractmethod
    def change_height(self, value: int):
        """Метод для изменения высоты (должен быть реализован в подклассах)."""
        pass

    @property
    def height(self):
        return self._height

    @property
    def color(self):
        return self._color

    @abstractmethod
    def __str__(self):
        """Абстрактный метод для строкового представления объекта."""
        pass


class Castle(BaseCastle):
    """Обычный замок, унаследованный от BaseCastle."""

    def change_height(self, value: int):
        """Изменяет высоту, не позволяя опуститься ниже 0."""
        self._height = max(0, self._height + value)

    def __str__(self):
        return f"The Castle at an altitude of {self._height} meters is {self._color}."


class Clouds:
    """Класс, представляющий облака, окружающие воздушный замок."""

    def __init__(self, count: int):
        self._count = max(0, count)

    def add_clouds(self, n: int):
        """Добавляет облака."""
        if n > 0:
            self._count += n

    def remove_clouds(self, n: int):
        """Удаляет облака, не позволяя их количеству стать отрицательным."""
        self._count = max(0, self._count - n)

    @property
    def count(self):
        return self._count

    def __str__(self):
        return f"{self._count} clouds"


class AirCastle(Castle):
    """Воздушный замок, содержащий облака (композиция)."""

    def __init__(self, height: int, cloud_count: int, color: str):
        super().__init__(height, color)
        self.clouds = Clouds(cloud_count)

    def __add__(self, n: int):
        """При сложении с числом добавляет n облаков и увеличивает высоту на n // 5."""
        new_clouds = Clouds(self.clouds.count + n)
        new_height = self.height + n // 5
        return AirCastle(new_height, new_clouds.count, self.color)

    def __call__(self, transparency: int):
        """При вызове экземпляра с числом рассчитывает видимость замка."""
        return (self.height // transparency) * self.clouds.count

    def __str__(self):
        """Возвращает строковое представление объекта."""
        return f"The AirCastle at an altitude of {self.height} meters is {self.color} with {self.clouds.count} clouds."

    def __repr__(self):
        return f"AirCastle({self.height}, {self.clouds.count}, '{self.color}')"

    def __lt__(self, other):
        return (self.clouds.count, self.height, self.color) < (other.clouds.count, other.height, other.color)

    def __le__(self, other):
        return (self.clouds.count, self.height, self.color) <= (other.clouds.count, other.height, other.color)

    def __gt__(self, other):
        return (self.clouds.count, self.height, self.color) > (other.clouds.count, other.height, other.color)

    def __ge__(self, other):
        return (self.clouds.count, self.height, self.color) >= (other.clouds.count, other.height, other.color)

    def __eq__(self, other):
        return (self.clouds.count, self.height, self.color) == (other.clouds.count, other.height, other.color)

    def __ne__(self, other):
        return (self.clouds.count, self.height, self.color) != (other.clouds.count, other.height, other.color)

    @property
    def cloud_count(self):
        return self.clouds.count

castle1 = AirCastle(10, 10, "red")
castle2 = AirCastle(8, 8, "green")

castle1.change_height(50)
castle1 = castle1 + 10
visibility = castle1(5)

print(castle1)
print(visibility)
print(castle1 > castle2)
