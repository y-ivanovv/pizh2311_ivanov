from player import AudioPlayer, VideoPlayer, DVDPlayer
from time import sleep

audio_player = AudioPlayer()
audio_player.start()
sleep(1)
audio_player.set_volume(75)
print(f"Текущая громкость: {audio_player.get_volume()}")
audio_player.stop()
print(f"Сэмпл: {audio_player.get_sample_rate()}")

print("\n")

video_player = VideoPlayer()
video_player.start()
sleep(1)
video_player.set_brightness(80)
print(f"Рзарешение видео: {video_player.get_resolution()}")
video_player.stop()

print("\n")

dvd_player = DVDPlayer()
dvd_player.start()
sleep(1)
dvd_player.insert_dvd("Звездные войны")
dvd_player.start()
dvd_player.stop()
dvd_player.eject_dvd()
dvd_player.eject_dvd()
print(f"DVD код региона: {dvd_player.get_region_code()}")
dvd_player.set_region_code("Регион 2")
print(f"DVD код региона: {dvd_player.get_region_code()}")

# Плеер запущен.
# Воспроизведение аудио.
# Громкость установлена на 75.
# Текущая громкость: 75
# Плеер остановлен. Текущая позиция: 1
# Аудио остановлено.
# Сэмпл: 44100


# Плеер запущен.
# Воспроизведение видео.
# Яркость установлена на 80.
# Рзарешение видео: 1920x1080
# Плеер остановлен. Текущая позиция: 1
# Видео остановлено.


# Устаовите DVD диск.
# DVD диск 'Звездные войны' установлен.
# Плеер запущен.
# Воспроизведение DVD: Звездные войны.
# Плеер остановлен. Текущая позиция: 0
# DVD остановлен.
# DVD диск 'Звездные войны' извлечен.
# DVD диск не установлен.
# DVD код региона: Region 1
# DVD код региона: Регион 2
