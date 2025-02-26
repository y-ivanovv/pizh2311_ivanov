from random import randint

class Warrior:
    number = 0
    def __init__(self, team):
        self.id = Warrior.number
        Warrior.number += 1
        self.team = team

class Soldier(Warrior):
    def __init__(self, team):
        Warrior.__init__(self, team)
        self.my_hero = None
    def follow(self, hero):
        self.my_hero = hero.id

class Hero(Warrior):
    def __init__(self, team):
        Warrior.__init__(self, team)
        self.level = 1
    def levelUp(self):
        self.level += 1

hero1 = Hero(1)
hero2 = Hero(2)

team1 = []
team2 = []

for i in range(11):
    n = randint(1, 2)
    if n == 1:
        team1.append(Soldier(n))
    else:
        team2.append(Soldier(n))

print(f"Первая команда: {len(team1)}; Вторая команда: {len(team2)}")

if len(team1) > len(team2):
    hero1.levelUp()
    print(f"Уровень первого героя повысился. Уровень первого героя: {hero1.level}")
else:
    hero2.levelUp()
    print(f"Уровень второго героя повысился. Уровень второго героя: {hero2.level}")

team1[0].follow(hero1)
print(f"id солдата, который следует за героем: {team1[0].id}; id первого героя: {hero1.id}")