from abc import ABC, abstractmethod

class IVector(ABC):
    @abstractmethod
    def __add__(self, other):
        pass
    
    @abstractmethod
    def __sub__(self, other):
        pass
    
    @abstractmethod
    def __mul__(self, scalar):
        pass
    
    @abstractmethod
    def magnitude(self):
        pass
    
    @abstractmethod
    def dot(self, other):
        pass

class IVectorSerializable(ABC):
    @abstractmethod
    def save(self, filename):
        pass
    
    @classmethod
    @abstractmethod
    def load(cls, filename):
        pass

class IVectorCollection(ABC):
    @abstractmethod
    def add(self, value):
        pass
    
    @abstractmethod
    def remove(self, index):
        pass
    
    @abstractmethod
    def __getitem__(self, index):
        pass
    
    @abstractmethod
    def __str__(self):
        pass

class IVectorCollectionSerializable(ABC):
    @abstractmethod
    def save(self, filename):
        pass
    
    @classmethod
    @abstractmethod
    def load(cls, filename):
        pass