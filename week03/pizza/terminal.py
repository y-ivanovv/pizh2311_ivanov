from pizza import PepperoniPizza, BarbecuePizza, SeafoodPizza
from order import Order

class Terminal:
    """Класс Terminal обеспечивает взаимодействие с клиентом."""

    COMPANY = "Пиццерия #1"  # Название компании
    CANCEL_COMMAND = -1      # Команда для отмены заказа
    CONFIRM_COMMAND = 0      # Команда для подтверждения заказа

    def __init__(self):
        """Конструктор класса.

        self.menu: список доступных пицц;
        self.order: текущий заказ;
        self.show_menu: определяет отображение меню
                        равен True: при создании терминала,
                        после отмены или подтверждения заказа.
        """
        # Доступные пиццы
        self.menu = [PepperoniPizza(), BarbecuePizza(), SeafoodPizza()]
        self.order = None
        self.show_menu = True

    def __str__(self):
        """Вернуть строковое представление класса.

        Формат вывода:

        Имя пиццерии, версия программы.
        """
        return f"{Terminal.COMPANY}, версия 1.0"

    def display_menu(self):
        """Показать меню.

        Показать меню следует только при наличии флага self.show_menu
        self.show_menu устанавливается в False после вывода меню.

        Формат вывода:

        Пиццерия #1
        Добро пожаловать!

        Меню:
        1. Пицца: Пепперони | Цена: 350.00 р.
           Тесто: тонкое Соус: томатный
           Начинка: пепперони, сыр моцарелла
        2. Пицца: Барбекю | Цена: 450.00 р.
           Тесто: тонкое Соус: барбекю
           Начинка: бекон, ветчина, зелень, сыр моцарелла
        3. Пицца: Дары моря | Цена: 550.00 р.
           Тесто: пышное Соус: тар-тар
           Начинка: кальмары, креветки, мидии, сыр моцарелла
        Для выбора укажите цифру через <ENTER>.
        Для отмены заказа введите -1
        Для подтверждения заказа введите 0
        """
        if not self.show_menu:
            return

        print(f"{Terminal.COMPANY}")
        print("Добро пожаловать!\n")
        print("Меню:")
        for i, pizza in enumerate(self.menu, 1):
            print(f"{i}. {pizza}")
        print("Для выбора укажите цифру через <ENTER>.")
        print("Для отмены заказа введите -1")
        print("Для подтверждения заказа введите 0")

        self.show_menu = False

    def process_command(self, menu_item):
        """Обработать действие пользователя.

        Аргументы:
          - menu_item (str): выбор пользователя.

        Возможные значения "menu_item":
          - -1: отменить заказ;
          -  0: подтвердить заказ; при этом осуществляется
                выставление счета, оплата, а также выполнение заказа;
                после заказ удаляется (= None)
          - 1..len(self.menu): добавление пиццы к заказу;
                               если заказ не создан, его нужно создать.
          - иначе: сообщить о невозможности обработать команду.

        Каждое действие подтверждается выводом на экран, например:
        1
        Пицца Пепперони добавлена!
        2
        Пицца Барбекю добавлена!
        0
        Заказ подтвержден.
        """
        try:
            menu_item = int(menu_item)
            if menu_item == Terminal.CANCEL_COMMAND:
                if self.order is not None:
                    self.order = None
                    print("Заказ отменен.")
                    self.show_menu = True
                else:
                    print("Нет активного заказа для отмены.")
            elif menu_item == Terminal.CONFIRM_COMMAND:
                if self.order is not None:
                    print("Заказ подтвержден.")
                    self.process_payment()
                    self.order.execute()
                    self.order = None
                    self.show_menu = True
                else:
                    print("Нет активного заказа для подтверждения.")
            elif 1 <= menu_item <= len(self.menu):
                if self.order is None:
                    self.order = Order()
                pizza = self.menu[menu_item - 1]
                self.order.add(pizza)
                print(f"Пицца {pizza.name} добавлена!")
            else:
                raise ValueError
        except ValueError:
            print("Не могу распознать команду! Проверьте ввод.")
        except Exception as e:
            print(f"Во время работы терминала произошла ошибка: {e}")

    def calculate_change(self, payment):
        """Вернуть сдачу для 'payment'.

        Если оплата меньше стоимости заказа, возбудить исключение ValueError.
        """
        if self.order is None:
            raise ValueError("Нет активного заказа для расчета сдачи.")
        total = self.order.total()
        if payment < total:
            raise ValueError("Оплата недостаточна.")
        return payment - total

    def process_payment(self):
        """Обработать оплату.

        Эмулирует оплату заказа (клиент вводит сумму с клавиатуры).

        Если сумма оплаты недостаточна (определяет метод calculate_change())
        или возникает другая ошибка - исключение передается выше.
        """
        try:
            if self.order is None:
                raise ValueError("Нет активного заказа для оплаты.")
            total = self.order.total()
            print(f"Сумма к оплате: {total:.2f} р.")
            payment = float(input("Введите сумму оплаты: "))
            change = self.calculate_change(payment)
            print(f"Сдача: {change:.2f} р.")
        except Exception as e:
            print(f"Оплата не удалась: {e}")
            raise
