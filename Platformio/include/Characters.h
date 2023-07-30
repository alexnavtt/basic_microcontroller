#pragma once

#include <stdint.h> 

namespace characters{

enum Alphabet : uint32_t {
    A = 0b00100'01010'11111'10001'10001,
    B = 0b11110'10001'11110'10001'11110,
    C = 0b01111'10000'10000'10000'01111,
    D = 0b11110'10001'10001'10001'11110,
    E = 0b11111'10000'11110'10000'11111,
    F = 0b11110'10000'11100'10000'10000,
    G = 0b01110'10000'10111'10001'01111,
    H = 0b10001'10001'11111'10001'10001,
    I = 0b01110'00100'00100'00100'01110,
    J = 0b01110'00100'00100'10100'11100,
    K = 0b10010'10100'11000'10100'10010,
    L = 0b10000'10000'10000'10000'11111,
    M = 0b10001'11011'10101'10001'10001,
    N = 0b10001'11001'10101'10011'10001,
    O = 0b01110'10001'10001'10001'01110,
    P = 0b11110'10001'11110'10000'10000,
    Q = 0b01110'10001'10101'10010'01101,
    R = 0b11110'10001'11110'10010'10001,
    S = 0b01111'10000'01110'00001'11110,
    T = 0b11111'00100'00100'00100'00100,
    U = 0b10001'10001'10001'10001'01110,
    V = 0b10001'10001'01010'01010'00100,
    W = 0b10001'10001'10101'11011'10001,
    X = 0b10001'01010'00100'01010'10001,
    Y = 0b10001'01010'00100'00100'00100,
    Z = 0b11111'00010'00100'01000'11111
};

enum Numerals : uint32_t {
    ZERO  = 0b01110'10011'10101'11001'01110,
    ONE   = 0b00100'01100'00100'00100'01110,
    TWO   = 0b01110'10001'00010'00100'11111,
    THREE = 0b01110'10001'00110'10001'01110,
    FOUR  = 0b10000'10010'11111'00010'00010,
    FIVE  = 0b11111'10000'11110'00001'11110,
    SIX   = 0b01110'10000'11110'10010'11110,
    SEVEN = 0b11111'00010'00100'01000'10000,
    EIGHT = 0b01110'10001'01110'10001'01110,
    NINE  = 0b01111'10001'01111'00001'00001
};

const uint32_t SPACE = 0x0000;
const uint32_t EXCLAMATION = 0b10000'10000'10000'00000'10000;

} // end namespace characters