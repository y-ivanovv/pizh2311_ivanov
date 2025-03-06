class Roman:
    """Класс Roman реализует работу с римскими числами.

    Алгоритм: http://math.hws.edu/eck/cs124/javanotes7/c8/ex3-ans.html.

    Внутри класс работает с обычными арабскими числами (int),
    которые преобразуются в римские при необходимости (например, при выводе).

    Ключевой атрибут: self._arabic (арабское число).

    Ограничения: число должно быть в пределах [1; 3999].
    """

    # Константы класса
    ARABIC_MIN = 1
    ARABIC_MAX = 3999
    ROMAN_MIN = "I"
    ROMAN_MAX = "MMMCMXCIX"

    LETTERS = ["M", "CM", "D", "CD", "C", "XC", "L",
               "XL", "X", "IX", "V", "IV", "I"]
    NUMBERS = [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1]

    def __init__(self, value):
        """Инициализация класса.

        Параметры:
            value (str): римское число, например, X.
                или
            value (int): арабское число, например, 5.
                или
            value (другой тип):  возбудить исключение TypeError.
        """
        if not isinstance(value, (int, str)):
            raise TypeError(f"Не могу создать римское число из {type(value)}")

        if isinstance(value, int):
            self.__check_arabic(value)
            self._arabic = value
        elif isinstance(value, str):
            self._arabic = self.to_arabic(value)

    def __add__(self, other):
        """Создать новый объект как сумму 'self' и 'other'."""
        if isinstance(other, Roman):
            return Roman(self._arabic + other._arabic)
        elif isinstance(other, int):
            return Roman(self._arabic + other)
        else:
            raise TypeError(f"Не могу сложить Roman с {type(other)}")

    def __sub__(self, other):
        """Создать новый объект как разность self и other."""
        if isinstance(other, Roman):
            return Roman(self._arabic - other._arabic)
        elif isinstance(other, int):
            return Roman(self._arabic - other)
        else:
            raise TypeError(f"Не могу вычесть {type(other)} из Roman")

    def __mul__(self, other):
        """Создать новый объект как произведение self и other."""
        if isinstance(other, Roman):
            return Roman(self._arabic * other._arabic)
        elif isinstance(other, int):
            return Roman(self._arabic * other)
        else:
            raise TypeError(f"Не могу умножить Roman на {type(other)}")

    def __floordiv__(self, other):
        """Создать новый объект как частное self и other."""
        if isinstance(other, Roman):
            return Roman(self._arabic // other._arabic)
        elif isinstance(other, int):
            return Roman(self._arabic // other)
        else:
            raise TypeError(f"Не могу разделить Roman на {type(other)}")

    def __truediv__(self, other):
        """Создать новый объект как частное self и other."""
        return self.__floordiv__(other)

    def __str__(self):
        """Вернуть строковое представление класса."""
        return Roman.to_roman(self._arabic)

    @staticmethod
    def __check_arabic(value):
        """Возбудить исключение ValueError, если 'value' не принадлежит
        [ARABIC_MIN; ARABIC_MIN]."""
        if not (Roman.ARABIC_MIN <= value <= Roman.ARABIC_MAX):
            raise ValueError(f"Арабское число {value} должно быть в пределах [{Roman.ARABIC_MIN}; {Roman.ARABIC_MAX}]")

    @staticmethod
    def __check_roman(value):
        """Возбудить исключение ValueError, если 'value' содержит
        недопустимые символы (не входящие в LETTERS)."""
        for char in value:
            if char not in Roman.LETTERS:
                raise ValueError(f"Недопустимый символ '{char}' в римском числе {value}")

    @property
    def arabic(self):
        """Вернуть арабское представление числа."""
        return self._arabic

    @staticmethod
    def to_arabic(roman):
        """Преобразовать римское число 'roman' в арабское.

        Параметры:
            roman (str): римское число, например, "X".

        Возвращает:
            int: арабское число.
        """
        def letter_to_number(letter):
            """Вернуть арабское значение римской цифры 'letter'.

            Регистр не учитывается."""
            roman_to_arabic = {
                'M': 1000,
                'D': 500,
                'C': 100,
                'L': 50,
                'X': 10,
                'V': 5,
                'I': 1
            }
            return roman_to_arabic[letter]

        Roman.__check_roman(roman)

        i = 0  # Позиция в строке roman
        value = 0  # Преобразованное число

        while i < len(roman):

            number = letter_to_number(roman[i])

            i += 1

            if i == len(roman):
                # В строке roman больше не осталось символов, добавляем number
                value += number
            else:
                # Если символы остались, необходимо посмотреть на следующий.
                # Если следующий символ "больше", считаем их за одну цифру.
                # Это необходимо, например, для того,
                # чтобы IV преобразовать в 4, а не 15.
                next_number = letter_to_number(roman[i])
                if next_number > number:
                    # Комбинируем цифры и перемещаем i к следующей
                    value += next_number - number
                    i += 1
                else:
                    # Просто добавляем следующую цифру
                    value += number

        Roman.__check_arabic(value)
        return value

    @staticmethod
    def to_roman(arabic):
        """Преобразовать арабское число 'arabic' в римское.

        Параметры:
            arabic (int): арабское число, например, 5.

        Возвращает:
            str: римское число.
        """
        Roman.__check_arabic(arabic)

        roman = ""
        # n - часть arabic, которую осталось преобразовать
        n = arabic

        for i, number in enumerate(Roman.NUMBERS):
            while n >= number:
                roman += Roman.LETTERS[i]
                n -= Roman.NUMBERS[i]

        return roman
