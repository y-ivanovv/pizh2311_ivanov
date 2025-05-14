from time import time

class Player:
    """
    Базовый класс для плееров, предоставляющий основные функции запуска и остановки.
    """
    def __init__(self):
        """
        Инициализирует объект Player.

        Атрибуты:
            color (str): Цвет плеера (общедоступный атрибут).
            _is_on (bool): Флаг, указывающий, включен ли плеер (необщедоступный атрибут).
            __current_position (int): Текущая позиция воспроизведения в секундах (закрытый атрибут).
            __start_time (float): Время запуска плеера (закрытый атрибут).
            __end_time (float): Время остановки плеера (закрытый атрибут).
        """
        self.color = "Зеленый"
        self._is_on = False
        self.__current_position = 0
        self.__start_time = None  # Инициализируем, чтобы не было ошибок при остановке до запуска
        self.__end_time = None  # Инициализируем, чтобы не было ошибок при остановке до запуска

    def start(self):
        """
        Запускает плеер.

        Если плеер еще не запущен, устанавливает флаг _is_on в True, записывает время запуска и выводит сообщение.
        Если плеер уже запущен, выводит сообщение о том, что плеер уже запущен.
        """
        if not self._is_on:
            self._is_on = True
            self.__start_time = time()
            print("Плеер запущен.")
        else:
            print("Плеер уже запущен.")

    def stop(self):
        """
        Останавливает плеер.

        Если плеер запущен, устанавливает флаг _is_on в False, записывает время остановки,
        вычисляет текущую позицию воспроизведения и выводит сообщение.
        Если плеер уже остановлен, выводит сообщение о том, что плеер уже остановлен.
        """
        if self._is_on:
            self._is_on = False
            self.__end_time = time()
            self.__current_position = self.__end_time - self.__start_time
            print(f"Плеер остановлен. Текущая позиция: {int(self.__current_position)}")
        else:
            print("Плеер уже остановлен.")


class AudioPlayer(Player):
    """
    Класс для аудио-плеера, наследуется от класса Player.
    """
    def __init__(self):
        """
        Инициализирует объект AudioPlayer.

        Атрибуты:
            _volume (int): Громкость плеера (необщедоступный атрибут).
            __sample_rate (int): Частота дискретизации аудио (закрытый атрибут).
        """
        super().__init__()
        self._volume = 50
        self.__sample_rate = 44100

    def start(self):
        """
        Запускает аудио-плеер.

        Вызывает метод start() родительского класса и выводит сообщение о воспроизведении аудио.
        """
        super().start()
        print("Воспроизведение аудио.")

    def stop(self):
        """
        Останавливает аудио-плеер.

        Вызывает метод stop() родительского класса и выводит сообщение об остановке аудио.
        """
        super().stop()
        print("Аудио остановлено.")

    def set_volume(self, volume):
        """
        Устанавливает громкость плеера.

        Args:
            volume (int): Новое значение громкости.
        """
        self._volume = volume
        print(f"Громкость установлена на {self._volume}.")

    def get_volume(self):
        """
        Возвращает текущую громкость плеера.

        Returns:
            int: Текущая громкость.
        """
        return self._volume

    def set_sample_rate(self, sample_rate):
        """
        Устанавливает частоту дискретизации аудио.

        Args:
            sample_rate (int): Новая частота дискретизации.
        """
        self.__sample_rate = sample_rate

    def get_sample_rate(self):
        """
        Возвращает текущую частоту дискретизации аудио.

        Returns:
            int: Текущая частота дискретизации.
        """
        return self.__sample_rate


class VideoPlayer(Player):
    """
    Класс для видео-плеера, наследуется от класса Player.
    """
    def __init__(self):
        """
        Инициализирует объект VideoPlayer.

        Атрибуты:
            video_property (str): Свойство видео (общедоступный атрибут).
            _brightness (int): Яркость плеера (необщедоступный атрибут).
            __resolution (str): Разрешение видео (закрытый атрибут).
        """
        super().__init__()
        self.video_property = "Video setting"
        self._brightness = 70
        self.__resolution = "1920x1080"

    def start(self):
        """
        Запускает видео-плеер.

        Вызывает метод start() родительского класса и выводит сообщение о воспроизведении видео.
        """
        super().start()
        print("Воспроизведение видео.")

    def stop(self):
        """
        Останавливает видео-плеер.

        Вызывает метод stop() родительского класса и выводит сообщение об остановке видео.
        """
        super().stop()
        print("Видео остановлено.")

    def set_brightness(self, brightness):
        """
        Устанавливает яркость плеера.

        Args:
            brightness (int): Новое значение яркости.
        """
        self._brightness = brightness
        print(f"Яркость установлена на {self._brightness}.")

    def get_resolution(self):
        """
        Возвращает текущее разрешение видео.

        Returns:
            str: Текущее разрешение видео.
        """
        return self.__resolution


class DVDPlayer(Player):
    """
    Класс для DVD-плеера, наследуется от класса Player.
    """
    def __init__(self):
        """
        Инициализирует объект DVDPlayer.

        Атрибуты:
            dvd_version (str): Версия DVD-плеера (общедоступный атрибут).
            _region_code (str): Региональный код DVD (необщедоступный атрибут).
            __dvd_disk (str): Название DVD-диска, установленного в плеере (закрытый атрибут).
        """
        super().__init__()
        self.dvd_version = "1.0"
        self._region_code = "Region 1"
        self.__dvd_disk = None

    def start(self):
        """
        Запускает DVD-плеер.

        Если в плеере установлен DVD-диск, вызывает метод start() родительского класса и выводит сообщение о воспроизведении DVD.
        Если в плеере нет DVD-диска, выводит сообщение с просьбой установить диск.
        """
        if self.__dvd_disk:
            super().start()
            print(f"Воспроизведение DVD: {self.__dvd_disk}.")
        else:
            print("Устаовите DVD диск.")

    def stop(self):
        """
        Останавливает DVD-плеер.

        Вызывает метод stop() родительского класса и выводит сообщение об остановке DVD.
        """
        super().stop()
        print("DVD остановлен.")

    def insert_dvd(self, dvd_disk):
        """
        Устанавливает DVD-диск в плеер.

        Args:
            dvd_disk (str): Название DVD-диска.
        """
        self.__dvd_disk = dvd_disk
        print(f"DVD диск '{dvd_disk}' установлен.")

    def eject_dvd(self):
        """
        Извлекает DVD-диск из плеера.
        """
        if self.__dvd_disk:
            disk = self.__dvd_disk
            self.__dvd_disk = None
            print(f"DVD диск '{disk}' извлечен.")
        else:
            print("DVD диск не установлен.")

    def get_region_code(self):
        """
        Возвращает региональный код DVD.

        Returns:
            str: Региональный код DVD.
        """
        return self._region_code

    def set_region_code(self, region_code):
        """
        Устанавливает региональный код DVD.

        Args:
            region_code (str): Новый региональный код.
        """
        self._region_code = region_code