from module import Room

print("Размеры комнаты:")
l = float(input("Длина - "))
w = float(input("Ширина - "))
h = float(input("Высота - "))
room = Room(l, w, h)

flag = input("Есть неоклеиваемая поверхность? (д / н) - ").lower()
while flag == "д":
    w = float(input("Ширина - "))
    h = float(input("Высота - "))
    room.addWD(w, h)
    flag = input("Добавить еще? (д / н) - ").lower()

print("Размеры рулона:")
l = float(input("Длина - "))
w = float(input("Ширина - "))

print("Площадь оклейки: ", room.workSurface())
print("Кол-во рулонов", room.wallpapers(l, w))