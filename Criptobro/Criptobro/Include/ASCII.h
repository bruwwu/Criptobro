#include "Prerequisites.h"

/**
 * @class AsciiBinary
 * @brief Utilidad para convertir entre texto ASCII y su representación binaria.
 *
 * Esta clase proporciona métodos para convertir cadenas de texto a binario y viceversa,
 * permitiendo codificar y decodificar mensajes de forma sencilla.
 */
class AsciiBinary {
public:
  /**
   * @brief Constructor por defecto.
   */
  AsciiBinary() = default;

  /**
   * @brief Destructor por defecto.
   */
  ~AsciiBinary() = default;

  /**
   * @brief Convierte una cadena de texto a su representación binaria.
   *
   * Cada carácter se convierte a un bloque de 8 bits, separado por espacios.
   *
   * @param input Cadena de texto a convertir.
   * @return Cadena binaria representando cada carácter.
   */
  std::string stringToBinary(const std::string& input) {
    std::ostringstream oss;
    for (char c : input) {
      oss << bitset(c) << " ";
    }
    return oss.str();
  }

  /**
   * @brief Convierte una cadena binaria de 8 bits a su carácter ASCII correspondiente.
   *
   * @param binary Cadena de 8 caracteres compuesta por '0' y '1'.
   * @return Carácter ASCII resultante.
   */
  char binaryToChar(const std::string& binary) {
    int value = 0;
    for (char bit : binary) {
      value = value * 2 + (bit - '0');
    }
    return static_cast<char>(value);
  }

  /**
   * @brief Convierte una cadena binaria completa a texto ASCII.
   *
   * La entrada debe tener bloques de 8 bits separados por espacios.
   *
   * @param binaryInput Cadena binaria completa.
   * @return Cadena de texto decodificada.
   */
  std::string binaryToString(const std::string& binaryInput) {
    std::istringstream iss(binaryInput);
    std::string result;
    std::string binary;
    while (iss >> binary) {
      result += binaryToChar(binary);
    }
    return result;
  }

private:
  /**
   * @brief Convierte un carácter a su representación binaria de 8 bits.
   *
   * @param c Carácter a convertir.
   * @return Cadena binaria de 8 caracteres ('0' y '1').
   */
  std::string bitset(char c) {
    std::string result(8, '0');
    int value = static_cast<unsigned char>(c);
    for (int i = 7; i >= 0; --i) {
      if (value % 2 == 1) {
        result[i] = '1';
      }
      value /= 2;
    }
    return result;
  }
};
