from vector import Vector

if __name__ == "__main__":
    # Создание векторов
    v1 = Vector(3, 4)
    v2 = Vector(1, 2)

    # Операции с векторами
    print(v1 + v2)  # Сложение
    print(v1 - v2)  # Вычитание
    print(v1 * 2)   # Умножение на скаляр
    print(v1.magnitude())  # Длина вектора
    print(v1.dot(v2))  # Скалярное произведение

    # Создание вектора из строки
    v3 = Vector.from_string("5,6")
    print(v3)

    # Сохранение и загрузка вектора
    v1.save('vector.json')
    v4 = Vector.load('vector.json')
    print(v4)

# Vector(4.0, 6.0)
# Vector(2.0, 2.0)
# Vector(6.0, 8.0)
# 5.0
# 11.0
# Vector(5.0, 6.0)
# Vector(3.0, 4.0)
