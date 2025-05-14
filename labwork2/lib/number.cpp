#include "number.h"
#include <cstring>
#include <algorithm>

uint2022_t from_uint(uint32_t value) {
    uint2022_t result;
    memset(&result, 0, sizeof(result));
    result.parts[0] = value;
    return result;
}

uint2022_t from_string(const char* str) {
    uint2022_t result = from_uint(0);
    while (*str) {
        if (isdigit(*str)) {
            result = result * from_uint(10) + from_uint(*str - '0');
        }
        str++;
    }
    return result;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;
    uint64_t carry = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t sum = (uint64_t)lhs.parts[i] + rhs.parts[i] + carry;
        result.parts[i] = sum & 0xFFFFFFFF;
        carry = sum >> 32;
    }
    return result;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;
    uint32_t borrow = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t diff = (uint64_t)lhs.parts[i] - rhs.parts[i] - borrow;
        borrow = (diff >> 32) ? 1 : 0;
        result.parts[i] = diff & 0xFFFFFFFF;
    }
    return result;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint64_t temp[128] = { 0 };
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint64_t product = (uint64_t)lhs.parts[i] * rhs.parts[j];
            temp[i + j] += product & 0xFFFFFFFF;
            temp[i + j + 1] += product >> 32;
        }
    }
    for (int k = 0; k < 127; k++) {
        temp[k + 1] += temp[k] >> 32;
        temp[k] &= 0xFFFFFFFF;
    }
    uint2022_t result;
    for (int k = 0; k < 64; k++) {
        result.parts[k] = temp[k] & 0xFFFFFFFF;
    }
    return result;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int i = 0; i < 64; i++) {
        if (lhs.parts[i] != rhs.parts[i]) return false;
    }
    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

// Вспомогательная функция: деление на 10 с остатком
static std::pair<uint2022_t, uint32_t> divmod10(const uint2022_t& num) {
    uint2022_t quotient;
    memset(&quotient, 0, sizeof(quotient));

    uint64_t remainder = 0;

    for (int i = 63; i >= 0; i--) {
        uint64_t current = (remainder << 32) | num.parts[i];
        uint64_t digit = current / 10;
        remainder = current % 10;

        quotient.parts[i] = static_cast<uint32_t>(digit);
    }

    return { quotient, static_cast<uint32_t>(remainder) };
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    if (value == from_uint(0)) {
        stream << "0";
        return stream;
    }

    char buffer[610]; // 2022 бит ~ 610 цифр в десятичной
    int index = 0;

    uint2022_t num = value;
    while (num != from_uint(0)) {
        auto [quotient, remainder] = divmod10(num);
        buffer[index++] = '0' + remainder;
        num = quotient;
    }

    std::reverse(buffer, buffer + index);
    buffer[index] = '\0';

    stream << buffer;
    return stream;
}
