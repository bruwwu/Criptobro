#include "Prerequisites.h"
#include "Vinegere.h"

int main() {
  // 1) Crear una clave de Vigenère
  std::string key = "SECRETMESSAGE";  // Esta es la clave que se usará para cifrar y descifrar
  Vignere vignere(key);

  // 2) Texto que será cifrado
  std::string plaintext = "HOLA MUNDO";

  // 3) Cifrar el texto usando la clave
  std::string ciphertext = vignere.encode(plaintext);
  std::cout << "Texto Cifrado: " << ciphertext << std::endl;

  // 4) Descifrar el texto usando la misma clave
  std::string decryptedText = vignere.decode(ciphertext);
  std::cout << "Texto Descifrado: " << decryptedText << std::endl;

  // 5) Probar un ataque de fuerza bruta para romper el cifrado
  std::string crackedKey = vignere.breakBruteForce(ciphertext);
  std::cout << "Clave Encontrada: " << crackedKey << std::endl;

  return 0;
}
