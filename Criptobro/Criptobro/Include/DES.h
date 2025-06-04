#include "Prerequisites.h"

class DES {
public:
  DES() = default;  // Constructor por defecto, no hace nada
  DES(const bitset<64>& key) : key(key) {
    generateSubkeys();  // Genera las subclaves al inicializar la clave
  }
  ~DES() = default;  // Destructor, no hace nada



  void generateSubkeys() {
    // Genera las subclaves a partir de la clave original de 64 bits
// Aqu� se deber�a implementar la l�gica para generar las 16 subclaves de 48 bits
// a partir de la clave de 64 bits, pero por simplicidad, no se implementa aqu�.
    for (int i = 0; i < 16; i++) {
      bitset<48> subkey((key.to_ullong() >> 1) & 0xFFFFFFFFFFFF);  // Crea una subclave de 48 bits (inicialmente vac�a)
      subkeys.push_back(subkey);  // Agrega subclaves vac�as (esto es solo un ejemplo)
    }
  }

  // 1. iPermutation: Este m�todo hace el primer cambio en los datos (texto claro) 
  // antes de empezar a cifrarlos. La permutaci�n reorganiza los bits del texto.
  bitset<64> iPermutation(const std::bitset<64>& input) {
    std::bitset<64> output;
    // Este ciclo copia cada bit de 'input' a 'output' (esto es solo una reorganizaci�n)
    for (int i = 0; i < 64; i++) {
      output[i] = input[i];  // Copia los bits de 'input' a 'output'
    }
    return output;  // Retorna los datos reorganizados (en este caso no cambia)
  }




  // 2. expand: Este m�todo toma un bloque de 32 bits y lo expande a 48 bits.
  // Es como agregar m�s "espacio" a los datos antes de hacer m�s operaciones.
  bitset<48> expand(const std::bitset<32>& halfBlock) {
    std::bitset<48> output;
    // Usa una tabla para decidir qu� bits copiar y c�mo expandir el bloque
    for (int i = 0; i < 48; i++) {
      output[i] = halfBlock[EXPANSION_TABLE[i] - 1];  // Toma bits de 'halfBlock' y los expande
    }
    return output;  // Devuelve el bloque expandido
  }




  // 3. substitute: Este m�todo toma 48 bits y los convierte en 32 bits usando una "tabla de sustituci�n".
  // B�sicamente, se busca un valor en la tabla y se reemplaza por otro, haciendo el cifrado m�s seguro.
  bitset<32> substitute(const std::bitset<48>& input) {
    std::bitset<32> output;
    // Divide los 48 bits en 8 bloques y usa la tabla S-Box para hacer sustituciones
    for (int i = 0; i < 8; i++) {
      int row = (input[i * 6] << 1) | input[i * 6 + 5];  // Calcula la fila en la tabla
      int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) |
        (input[i * 6 + 3] << 1) | input[i * 6 + 4];  // Calcula la columna

      int sboxValue = SBOX[row][col];  // Encuentra el valor en la tabla S-Box

      // Convierte el valor encontrado en bits y lo coloca en el bloque de salida
      for (int j = 0; j < 4; j++) {
        output[i * 4 + j] = (sboxValue >> (3 - j)) & 1;  // Extrae cada bit
      }
    }
    return output;  // Devuelve el bloque de 32 bits con la sustituci�n
  }



  // 4. feistel: Este es el proceso m�s importante en el cifrado DES. Toma 32 bits, los expande,
  // hace un XOR con una subclave y luego hace una sustituci�n.
  bitset<32> feistel(const std::bitset<32>& right, const std::bitset<48>& subkey) {
    // Expande el bloque de 32 bits a 48
    auto expanded = expand(right);

    // Realiza una operaci�n XOR (como un "mix" de los bits)
    auto xored = expanded ^ subkey;

    // Sustituye los bits usando la S-Box
    auto substituted = substitute(xored);

    return substituted;  // Devuelve los 32 bits resultantes
  }



  // 5. encode: Este m�todo toma un bloque de 64 bits (texto claro) y lo cifra usando 16 rondas.
  // Divide el bloque en dos partes y realiza la funci�n Feistel en cada parte durante 16 rondas.
  bitset<64> encode(std::bitset<64>& plaintext) {
    // Aplica la permutaci�n inicial
    auto data = iPermutation(plaintext);

    // Divide el bloque de 64 bits en dos mitades de 32 bits
    std::bitset<32> left(data.to_ulong() >> 32);  // Primeros 32 bits
    std::bitset<32> right(data.to_ulong());  // �ltimos 32 bits

    // Realiza 16 rondas del algoritmo Feistel
    for (int round = 0; round < 16; round++) {
      // Aplica Feistel y mezcla la mitad izquierda con la derecha
      auto newRight = left ^ feistel(right, subkeys[round]);
      left = right;  // La mitad derecha se convierte en la nueva mitad izquierda
      right = newRight;  // La nueva derecha es el resultado de Feistel
    }

    // Combina las mitades para formar el texto cifrado final
    std::bitset<64> result;
    result = left.to_ullong() << 32 | right.to_ullong();  // Une ambas mitades
    return result;  // Devuelve el bloque de 64 bits cifrado
  }



  // 6. premuteP: Este m�todo toma un bloque de 32 bits y lo reorganiza seg�n una tabla de permutaci�n.
  // Es como un �ltimo paso para mezclar los bits antes de finalizar el cifrado.
  // Este paso se realiza despu�s de las 16 rondas de cifrado.
  bitset<32> premuteP(const bitset<32>& input) {
    bitset<32> output;  // Creamos un nuevo bitset para guardar el resultado de la permutaci�n

    // Aplica la permutaci�n P a los 32 bits de entrada
    // La idea aqu� es reorganizar los bits de 'input' seg�n una tabla de permutaci�n.
    for (int i = 0; i < 32; i++) {
      // 'P_TABLE' es una tabla que nos dice en qu� orden poner los bits
      output[i] = input[P_TABLE[i] - 1];  // Tomamos el bit de la posici�n indicada en P_TABLE y lo colocamos en 'output'
    }

    // Devolvemos el resultado, que es un conjunto de 32 bits reorganizados
    return output;  // Los 32 bits est�n ahora en el nuevo orden
  }

  bitset<64>
    fPremutation(const bitset<64>& input) {
    bitset<64> output;  // Creamos un nuevo bitset para guardar el resultado de la permutaci�n
    for (int i = 0; i < 64; i++)
    {
      output[i] = input[i];  // Copia los bits de 'input' a 'output' (esto es solo una reorganizaci�n)
    }
    return output;
  }


private:
  std::bitset<64> key;  // La clave de 64 bits
  std::vector<std::bitset<48>> subkeys;  // Las subclaves de 48 bits para cada ronda

  // Tabla de expansi�n (E) para ampliar los bloques de 32 bits a 48 bits
  const int EXPANSION_TABLE[48] = {
      32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
      8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
      16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
      24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
  };

  // Tabla de permutaci�n (P) para la funci�n final de permutaci�n
  const int P_TABLE[32] = {
      16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26,
      5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9,
      19, 13, 30, 6, 22, 11, 4, 25
  };

  // S-Box (simplificada) para la sustituci�n de 6 bits por 4 bits
  const int SBOX[4][16] = {
      {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
      {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
      {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
      {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
  };
};
