from module import *

lesson = Data("class", "object", "inheritance", "polymorphism", "encapsulation")
pupil = Pupil()
for i in lesson:
    pupil.learn(i)
print(pupil.knowledge)
pupil.forget()
print(pupil.knowledge)
pupil.forget()
print(pupil.knowledge)