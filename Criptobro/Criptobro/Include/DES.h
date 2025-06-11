#include "Prerequisites.h"

/**
 * @class DES
 * @brief Implementación básica del algoritmo de cifrado DES (Data Encryption Standard).
 *
 * Esta clase incluye la estructura fundamental del algoritmo DES, incluyendo generación de subclaves,
 * permutaciones, expansión, sustitución (S-Box) y la función Feistel. No es una implementación
 * completa y segura, pero sirve como una base didáctica.
 */
class DES {
public:
  DES() = default;

  /**
   * @brief Constructor que recibe una clave de 64 bits y genera las subclaves.
   * @param key Clave inicial en forma de `std::bitset<64>`.
   */
  DES(const bitset<64>& key) : key(key) {
    generateSubkeys();
  }

  ~DES() = default;

  /**
   * @brief Genera 16 subclaves de 48 bits a partir de la clave original de 64 bits.
   */
  void generateSubkeys() {
    for (int i = 0; i < 16; i++) {
      bitset<48> subkey((key.to_ullong() >> 1) & 0xFFFFFFFFFFFF);
      subkeys.push_back(subkey);
    }
  }

  /**
   * @brief Aplica la permutación inicial a un bloque de 64 bits.
   * @param input Bloque original.
   * @return Bloque permutado.
   */
  bitset<64> iPermutation(const bitset<64>& input) {
    bitset<64> output;
    for (int i = 0; i < 64; i++) {
      output[i] = input[i]; // (placeholder)
    }
    return output;
  }

  /**
   * @brief Expande un bloque de 32 bits a 48 bits usando una tabla de expansión.
   * @param halfBlock Bloque de 32 bits.
   * @return Bloque expandido de 48 bits.
   */
  bitset<48> expand(const bitset<32>& halfBlock) {
    bitset<48> output;
    for (int i = 0; i < 48; i++) {
      output[i] = halfBlock[EXPANSION_TABLE[i] - 1];
    }
    return output;
  }

  /**
   * @brief Sustituye un bloque de 48 bits por 32 bits utilizando una S-Box simplificada.
   * @param input Bloque de 48 bits.
   * @return Bloque de 32 bits tras la sustitución.
   */
  bitset<32> substitute(const bitset<48>& input) {
    bitset<32> output;
    for (int i = 0; i < 8; i++) {
      int row = (input[i * 6] << 1) | input[i * 6 + 5];
      int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) |
        (input[i * 6 + 3] << 1) | input[i * 6 + 4];
      int sboxValue = SBOX[row][col];
      for (int j = 0; j < 4; j++) {
        output[i * 4 + j] = (sboxValue >> (3 - j)) & 1;
      }
    }
    return output;
  }

  /**
   * @brief Función Feistel: expansión, XOR con subclave, sustitución.
   * @param right Parte derecha del bloque de datos.
   * @param subkey Subclave de 48 bits correspondiente a la ronda.
   * @return Resultado de aplicar la función Feistel.
   */
  bitset<32> feistel(const bitset<32>& right, const bitset<48>& subkey) {
    auto expanded = expand(right);
    auto xored = expanded ^ subkey;
    auto substituted = substitute(xored);
    return substituted;
  }

  /**
   * @brief Cifra un bloque de 64 bits mediante 16 rondas de la estructura Feistel.
   * @param plaintext Bloque de entrada (texto claro).
   * @return Bloque cifrado de 64 bits.
   */
  bitset<64> encode(bitset<64>& plaintext) {
    auto data = iPermutation(plaintext);
    bitset<32> left(data.to_ullong() >> 32);
    bitset<32> right(data.to_ullong());

    for (int round = 0; round < 16; round++) {
      auto newRight = left ^ feistel(right, subkeys[round]);
      left = right;
      right = newRight;
    }

    bitset<64> result;
    result = left.to_ullong();
    result <<= 32;
    result |= right.to_ullong();
    return result;
  }

  /**
   * @brief Aplica una permutación P a un bloque de 32 bits.
   * @param input Bloque de entrada.
   * @return Bloque permutado según la tabla P.
   */
  bitset<32> premuteP(const bitset<32>& input) {
    bitset<32> output;
    for (int i = 0; i < 32; i++) {
      output[i] = input[P_TABLE[i] - 1];
    }
    return output;
  }

  /**
   * @brief Permutación final (placeholder) después de cifrar.
   * @param input Bloque cifrado.
   * @return Bloque reorganizado (actualmente sin cambios).
   */
  bitset<64> fPremutation(const bitset<64>& input) {
    bitset<64> output;
    for (int i = 0; i < 64; i++) {
      output[i] = input[i];
    }
    return output;
  }

private:
  bitset<64> key; ///< Clave original de 64 bits.
  std::vector<bitset<48>> subkeys; ///< Subclaves generadas para las 16 rondas.

  /// Tabla de expansión de 32 bits a 48 bits.
  const int EXPANSION_TABLE[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
  };

  /// Tabla de permutación P de 32 bits.
  const int P_TABLE[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26,
    5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
  };

  /// Tabla S-Box simplificada para sustitución (4x16).
  const int SBOX[4][16] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
  };
};
