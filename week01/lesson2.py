from random import randint

class Warrior:
    def setHealth(self, hp):
        self.health = hp
    def kick(enemy):
        enemy.health -= 20

warrior_1 = Warrior()
warrior_2 = Warrior()
warrior_1.setHealth(100)
warrior_2.setHealth(100)

while warrior_1.health > 0 and warrior_2.health > 0:
    n = randint(1,2)
    if n == 1:
        Warrior.kick(warrior_2)
        print(f"Первый юнит атакует второго. Уровень здоровья второго юнита: {warrior_2.health}")
    elif n == 2:
        Warrior.kick(warrior_1)
        print(f"Второй юнит атакует первого. Уровень здоровья первого юнита: {warrior_1.health}")

if warrior_1.health == 0:
    print("Второй юнит победил!")
else:
    print("Первый юнит победил!")