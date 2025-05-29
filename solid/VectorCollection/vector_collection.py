import json
from interfaces import IVectorCollection, IVectorCollectionSerializable
from vector import Vector

class VectorCollection(IVectorCollection, IVectorCollectionSerializable):
    def __init__(self):
        self._data = []

    def __str__(self):
        return "\n".join(str(vector) for vector in self._data)

    def __getitem__(self, index):
        return self._data[index]

    def add(self, value):
        if isinstance(value, Vector):
            self._data.append(value)
        else:
            raise TypeError("Можно добавлять только объекты типа Vector.")

    def remove(self, index):
        if 0 <= index < len(self._data):
            self._data.pop(index)
        else:
            raise IndexError("Индекс вне диапазона.")

    def save(self, filename):
        with open(filename, 'w') as file:
            json.dump([{'x': v.x, 'y': v.y} for v in self._data], file)

    @classmethod
    def load(cls, filename):
        with open(filename, 'r') as file:
            data = json.load(file)
        collection = cls()
        for item in data:
            collection.add(Vector(item['x'], item['y']))
        return collection