from random import randrange

class Data:
    def __init__(self, *info):
        self.info = list(info)
    def __getitem__(self, i):
        return self.info[i]
    
class Teacher:
    def teach(self, info, *pupil):
        for i in pupil:
            i.learn(info)

class Pupil:
    def __init__(self):
        self.knowledge = []
    def learn(self, info):
        self.knowledge.append(info)
    def forget(self):
        i = randrange(len(self.knowledge))
        del self.knowledge[i]