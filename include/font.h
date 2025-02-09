// font.h
// Fontes para: Nothing, dígitos (0–9), letras maiúsculas (A–Z) e letras minúsculas (a–z).
// Cada caractere é representado por 8 bytes, onde cada byte representa uma coluna (de cima para baixo).
// As letras minúsculas foram transpostas (de linha para coluna) e depois invertidas para que fiquem com a mesma orientação que os demais.

static uint8_t font[] = {
  // Grupo 0: "Nothing" (caractere vazio)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Nothing

  // Grupo 1: Dígitos 0-9
  0x3e, 0x41, 0x41, 0x49, 0x41, 0x41, 0x3e, 0x00, // 0
  0x00, 0x00, 0x42, 0x7f, 0x40, 0x00, 0x00, 0x00, // 1
  0x30, 0x49, 0x49, 0x49, 0x49, 0x46, 0x00, 0x00, // 2
  0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, // 3
  0x3f, 0x20, 0x20, 0x78, 0x20, 0x20, 0x00, 0x00, // 4
  0x4f, 0x49, 0x49, 0x49, 0x49, 0x30, 0x00, 0x00, // 5
  0x3f, 0x48, 0x48, 0x48, 0x48, 0x48, 0x30, 0x00, // 6
  0x01, 0x01, 0x01, 0x61, 0x31, 0x0d, 0x03, 0x00, // 7
  0x36, 0x49, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, // 8
  0x06, 0x09, 0x09, 0x09, 0x09, 0x09, 0x7f, 0x00, // 9

  // Grupo 2: Letras maiúsculas A-Z
  0x78, 0x14, 0x12, 0x11, 0x12, 0x14, 0x78, 0x00, // A
  0x7f, 0x49, 0x49, 0x49, 0x49, 0x49, 0x7f, 0x00, // B
  0x7e, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x00, // C
  0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7e, 0x00, // D
  0x7f, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x00, // E
  0x7f, 0x09, 0x09, 0x09, 0x09, 0x01, 0x01, 0x00, // F
  0x7f, 0x41, 0x41, 0x41, 0x51, 0x51, 0x73, 0x00, // G
  0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x7f, 0x00, // H
  0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, // I
  0x21, 0x41, 0x41, 0x3f, 0x01, 0x01, 0x01, 0x00, // J
  0x00, 0x7f, 0x08, 0x08, 0x14, 0x22, 0x41, 0x00, // K
  0x7f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, // L
  0x7f, 0x02, 0x04, 0x08, 0x04, 0x02, 0x7f, 0x00, // M
  0x7f, 0x02, 0x04, 0x08, 0x10, 0x20, 0x7f, 0x00, // N
  0x3e, 0x41, 0x41, 0x41, 0x41, 0x41, 0x3e, 0x00, // O
  0x7f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00, // P
  0x3e, 0x41, 0x41, 0x49, 0x51, 0x61, 0x7e, 0x00, // Q
  0x7f, 0x11, 0x11, 0x11, 0x31, 0x51, 0x0e, 0x00, // R
  0x46, 0x49, 0x49, 0x49, 0x49, 0x30, 0x00, 0x00, // S
  0x01, 0x01, 0x01, 0x7f, 0x01, 0x01, 0x01, 0x00, // T
  0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3f, 0x00, // U
  0x0f, 0x10, 0x20, 0x40, 0x20, 0x10, 0x0f, 0x00, // V
  0x7f, 0x20, 0x10, 0x08, 0x10, 0x20, 0x7f, 0x00, // W
  0x00, 0x41, 0x22, 0x14, 0x14, 0x22, 0x41, 0x00, // X
  0x01, 0x02, 0x04, 0x78, 0x04, 0x02, 0x01, 0x00, // Y
  0x41, 0x61, 0x59, 0x45, 0x43, 0x41, 0x00, 0x00, // Z

  // Grupo 3: Letras minúsculas a-z (transpostas e invertidas)
  0x00, 0x20, 0x54, 0x54, 0x54, 0x34, 0x78, 0x00, // a
  0x00, 0x7E, 0x50, 0x48, 0x48, 0x48, 0x30, 0x00, // b
  0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x28, 0x00, // c
  0x00, 0x30, 0x48, 0x48, 0x48, 0x50, 0x7E, 0x00, // d
  0x00, 0x38, 0x54, 0x54, 0x54, 0x54, 0x18, 0x00, // e
  0x00, 0x00, 0x08, 0x7C, 0x0A, 0x0A, 0x00, 0x00, // f
  0x00, 0x48, 0x94, 0x94, 0x94, 0xB4, 0x78, 0x00, // g
  0x00, 0x7E, 0x10, 0x08, 0x08, 0x08, 0x70, 0x00, // h
  0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, // i
  0x00, 0x60, 0x40, 0x74, 0x00, 0x00, 0x00, 0x00, // j
  0x00, 0x7E, 0x08, 0x1C, 0x32, 0x42, 0x00, 0x00, // k
  0x00, 0x3E, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, // l
  0x00, 0x00, 0x78, 0x04, 0x78, 0x04, 0x78, 0x00, // m
  0x00, 0x00, 0x00, 0x04, 0x78, 0x04, 0x78, 0x00, // n
  0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, // o
  0x00, 0xFC, 0x24, 0x24, 0x24, 0x18, 0x00, 0x00, // p
  0x00, 0x18, 0x24, 0x24, 0x24, 0xFC, 0x00, 0x00, // q
  0x00, 0x78, 0x10, 0x08, 0x08, 0x08, 0x00, 0x00, // r
  0x00, 0x48, 0x54, 0x54, 0x24, 0x00, 0x00, 0x00, // s
  0x00, 0x08, 0x7C, 0x08, 0x08, 0x08, 0x00, 0x00, // t
  0x00, 0x3C, 0x40, 0x40, 0x40, 0x20, 0x7C, 0x00, // u
  0x00, 0x1C, 0x20, 0x40, 0x40, 0x20, 0x1C, 0x00, // v
  0x00, 0x7C, 0x40, 0x30, 0x30, 0x40, 0x7C, 0x00, // w
  0x00, 0x44, 0x28, 0x10, 0x10, 0x28, 0x44, 0x00, // x
  0x00, 0x0C, 0x10, 0x60, 0x60, 0x10, 0x0C, 0x00, // y
  0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00  // z
};
