from vector import Vector
from vector_collection import VectorCollection

# Создание векторов
v1 = Vector(3, 4)
v2 = Vector(1, 2)
v3 = Vector(5, 6)

# Создание контейнера и добавление векторов
collection = VectorCollection()
collection.add(v1)
collection.add(v2)
collection.add(v3)

# Вывод содержимого контейнера
print("Содержимое контейнера:")
print(collection)

# Индексация
print("\nПервый элемент контейнера:")
print(collection[0])

# Удаление элемента
collection.remove(1)
print("\nСодержимое контейнера после удаления второго элемента:")
print(collection)

# Сохранение контейнера в файл
collection.save('vectors.json')

# Загрузка контейнера из файла
loaded_collection = VectorCollection.load('vectors.json')
print("\nСодержимое загруженного контейнера:")
print(loaded_collection)

# Содержимое контейнера:
# Vector(3.0, 4.0)
# Vector(1.0, 2.0)
# Vector(5.0, 6.0)

# Первый элемент контейнера:
# Vector(3.0, 4.0)

# Содержимое контейнера после удаления второго элемента:
# Vector(3.0, 4.0)
# Vector(5.0, 6.0)

# Содержимое загруженного контейнера:
# Vector(3.0, 4.0)
# Vector(5.0, 6.0)
