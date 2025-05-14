from roman import Roman

if __name__ == "__main__":

    r1 = Roman("X")
    r2 = Roman(5)

    print("       Числа:", r1, r2, r1.arabic, r2.arabic)
    print("       Сумма:", r1 + r2)
    print("    Разность:", r1 - r2)
    print("Произведение:", r1 * r2)
    print("     Частное:", r1 // r2)

    print("\nПреобразование без создания объекта:")
    print(2016, "=", Roman.to_roman(2016))
    print("MMXVI", "=", Roman.to_arabic("MMXVI"))

# -------------
# Пример вывода:

#        Числа: X V 10 5
#        Сумма: XV
#     Разность: V
# Произведение: L
#      Частное: II
#
# Преобразование без создания объекта:
# 2016 = MMXVI
# MMXVI = 2016
