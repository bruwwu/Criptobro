#include "Prerequisites.h"
#include "Generator.h"

int main() {
  Generator cryptoGen;

  // 1) Generar un conjunto de contraseñas
  std::vector<std::string> passwords;
  for (int i = 0; i < 10; ++i) {  // Generamos 10 contraseñas para este ejemplo
    passwords.push_back(cryptoGen.generatePassword(16));
  }

  // 2) Evaluar la fuerza de cada contraseña
  std::vector<std::pair<std::string, std::string>> passwordStrengths;  // (Contraseña, Fuerza)
  for (const auto& password : passwords) {
    std::string strength = cryptoGen.passwordStrength(password);
    passwordStrengths.push_back({ password, strength });
  }

  // 3) Ordenar las contraseñas por su fuerza (de más fuerte a más débil)
  std::sort(passwordStrengths.begin(), passwordStrengths.end(),
    [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
      return a.second > b.second;  // Ordenar de mayor a menor fuerza
    });

  // 4) Mostrar las tres contraseñas más fuertes
  std::cout << "Las 3 contraseñas más fuertes son:\n";
  for (int i = 0; i < 3; ++i) {
    std::cout << "Contraseña: " << passwordStrengths[i].first << " | Fuerza: " << passwordStrengths[i].second << std::endl;
  }

  return 0;
}
