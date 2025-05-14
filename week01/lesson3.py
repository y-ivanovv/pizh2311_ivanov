class Person:
    def __init__(self, name, surname, qualification = 1):
        self.name = name
        self.surname = surname
        self.qualification = qualification
    def __del__(self):
        print(f"До свидания мистер {self.surname} {self.name}")
    def info(self):
        return "{0} {1}, {2}".format(self.surname, self.name, self.qualification)
  
person1 = Person("Юрий", "Иванов", 3)
person2 = Person("Ярослав", "Петрищев", 2)
person3 = Person("Савелий", "Пронченко")
print(person1.info())
print(person2.info())
print(person3.info())
del person3
input()