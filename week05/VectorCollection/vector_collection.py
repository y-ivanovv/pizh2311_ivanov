import json
from vector import Vector  # Импортируем класс Vector из предыдущей задачи

class VectorCollection:
    def __init__(self):
        """
        Инициализирует контейнер VectorCollection.

        Этот метод создает пустой список (_data) для хранения объектов Vector.
        """
        self._data = []

    def __str__(self):
        """
        Возвращает строковое представление контейнера.

        Этот метод преобразует каждый вектор в контейнере в строку с помощью str(vector)
        и объединяет их в одну строку, разделенную символами новой строки.

        Returns:
            str: Строковое представление контейнера, где каждый вектор представлен на отдельной строке.
        """
        return "\n".join(str(vector) for vector in self._data)

    def __getitem__(self, index):
        """
        Поддержка индексации и срезов для доступа к элементам контейнера.

        Этот метод позволяет получать доступ к элементам контейнера по их индексу,
        а также создавать срезы контейнера.

        Args:
            index (int): Индекс элемента, к которому нужно получить доступ.  Может быть отрицательным.
                        Также может быть объект slice для получения среза списка.

        Returns:
            Vector: Объект Vector, расположенный по указанному индексу.  В случае среза, возвращает новый VectorCollection.

        Raises:
            IndexError: Если индекс находится вне допустимого диапазона.
        """
        return self._data[index]

    def add(self, value):
        """
        Добавляет элемент в контейнер.

        Этот метод добавляет объект Vector в конец списка _data, если переданный объект является экземпляром класса Vector.

        Args:
            value (Vector): Объект Vector, который нужно добавить в контейнер.

        Raises:
            TypeError: Если переданный аргумент не является объектом Vector.
        """
        if isinstance(value, Vector):
            self._data.append(value)
        else:
            raise TypeError("Можно добавлять только объекты типа Vector.")

    def remove(self, index):
        """
        Удаляет элемент из контейнера по индексу.

        Этот метод удаляет элемент из списка _data, расположенный по указанному индексу.

        Args:
            index (int): Индекс элемента, который нужно удалить.

        Raises:
            IndexError: Если индекс находится вне допустимого диапазона.
        """
        if 0 <= index < len(self._data):
            self._data.pop(index)
        else:
            raise IndexError("Индекс вне диапазона.")

    def save(self, filename):
        """
        Сохраняет контейнер в JSON-файл.

        Этот метод преобразует список объектов Vector в JSON-формат и записывает его в файл.
        Каждый объект Vector преобразуется в словарь с ключами 'x' и 'y',
        соответствующими координатам вектора.

        Args:
            filename (str): Имя файла, в который нужно сохранить контейнер.
        """
        with open(filename, 'w') as file:
            json.dump([{'x': v.x, 'y': v.y} for v in self._data], file)

    @classmethod
    def load(cls, filename):
        """
        Загружает контейнер из JSON-файла.

        Этот метод создает новый экземпляр класса VectorCollection и заполняет его данными,
        загруженными из JSON-файла. Каждый объект в файле должен быть словарем с ключами 'x' и 'y',
        соответствующими координатам вектора.

        Args:
            filename (str): Имя файла, из которого нужно загрузить контейнер.

        Returns:
            VectorCollection: Новый экземпляр VectorCollection, заполненный данными из файла.
        """
        with open(filename, 'r') as file:
            data = json.load(file)
        collection = cls()
        for item in data:
            collection.add(Vector(item['x'], item['y']))
        return collection