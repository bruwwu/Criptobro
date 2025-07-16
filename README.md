# Criptobro CLI – Manual de Usuario

> **Versión:** 1.0  ·  **Fecha:** 15‑jul‑2025
> **Autor:** Bruwu
---

## Índice

1. [¿Qué es Criptobro CLI?](#que-es-criptobro-cli)
2. [Requisitos](#requisitos)
3. [Estructura de Carpetas](#estructura-de-carpetas)
4. [Compilación](#compilacion)
5. [Uso Rápido](#uso-rapido)
6. [Detalle de Algoritmos](#detalle-de-algoritmos)
7. [Ejemplos Paso a Paso](#ejemplos)
8. [Solución de Problemas](#solucion-de-problemas)
9. [Seguridad y Buenas Prácticas](#seguridad)
10. [Extensiones Futuras](#extensiones-futuras)
11. [Licencia](#licencia)

---

## 1. ¿Qué es Criptobro CLI?<a name="que-es-criptobro-cli"></a>

Criptobro CLI es una aplicación de consola escrita en C++ 17 que **cifra y descifra archivos** mediante cuatro algoritmos:

| # | Algoritmo         | Extensión salida | Clave predeterminada |
| - | ----------------- | ---------------- | -------------------- |
| 1 | **César**         | `.csr`           | −                    |
| 2 | **XOR**           | `.xor`           | Usuario              |
| 3 | **Vigenère**      | `.vig`           | Usuario              |
| 4 | **DES** (64 bits) | `.des`           | Fija en código       |

Todos los archivos **crudos** se almacenan en la carpeta `raw/` y los **cifrados** en `cif/`, garantizando que nunca se mezclen.

---

## 2. Requisitos<a name="requisitos"></a>

| Software               | Versión mínima                     | Notas                                  |
| ---------------------- | ---------------------------------- | -------------------------------------- |
| **Compilador C++**     | C++17 (g++ 7, clang 5, MSVC 19.14) | Soporte `std::filesystem`.             |
| **OpenSSL** (opcional) | 1.1                                | Sólo si activas SHA‑256 en `FileIO.h`. |

> **Windows + MSVC**: activa `/std:c++17` en *Project → Properties → C/C++ → Language*.

---

## 3. Estructura de Carpetas<a name="estructura-de-carpetas"></a>

```
Criptobro/                 <- raíz del proyecto
├─ Include/                <- headers (*.h)
├─ Source/                 <- main.cpp, etc.
├─ raw/                    <- ⚠️ archivos originales (entrada)
└─ cif/                    <- 🔒 archivos cifrados (salida)
```

Las carpetas `raw/` y `cif/` se crean automáticamente al ejecutar por primera vez.

---

## 4. Compilación<a name="compilacion"></a>

### Linux / macOS / WSL

```bash
cd Criptobro
g++ -std=c++17 -O2 Source/main.cpp -o crypto_cli
```

### Windows (MSVC)

```powershell
cd Criptobro
cl /std:c++17 /EHsc /O2 Source\main.cpp /I Include /link /OUT:crypto_cli.exe
```

> Si usas **vcpkg** para OpenSSL: `vcpkg install openssl:x64-windows` y añade `/link libcrypto.lib`.

---

## 5. Uso Rápido<a name="uso-rapido"></a>

```text
$ ./crypto_cli
Elige algoritmo:
  1) César
  2) XOR
  3) Vigenère
  4) DES (64-bit, clave fija)
> 2
¿Encrypt (e) o Decrypt (d)? > e
Nombre de archivo (sin ruta) > carta.txt
Leyendo de : .../raw/carta.txt
Escribiendo: .../cif/carta.txt.xor
Clave XOR > MiSecreta123
✅ Bytes escritos: 1024
```

*Para descifrar, repite el proceso con opción **d** y el mismo nombre + extensión.*

---

## 6. Detalle de Algoritmos<a name="detalle-de-algoritmos"></a>

### 6.1  César

* **Entrada**: desplazamiento entero (positivo o negativo).
* **Tip**: USA −13 para rot13.

### 6.2  XOR

* **Entrada**: cualquier cadena ASCII (se repite sobre el archivo).
* **Propiedad**: aplicar XOR dos veces con la misma clave revierte el texto.

### 6.3  Vigenère

* **Entrada**: palabra/frase sin espacios (A‑Z o a‑z).
* **Recomendación**: mezcla mayúsculas, minúsculas y números.

### 6.4  DES (ECB)

* **Clave fija**: `0001001100110100010101110111100110011011101111001101111111110001`.
* **Bloque**: 64 bits; última porción se rellena con `\0`.
* **Modo**: ECB puro (no IV). *No apto para datos sensibles en producción.*

---

## 7. Ejemplos Paso a Paso<a name="ejemplos"></a>

### 7.1  Cifrar imagen con DES

```text
# Copia moon.jpg dentro de raw/
./crypto_cli
  > 4   (DES)
  > e   (Encrypt)
  > moon.jpg
✅ crea cif/moon.jpg.des
```

### 7.2  Descifrar ese archivo

```text
./crypto_cli
  > 4
  > d
  > moon.jpg.des
✅ recupera raw/moon.jpg
```

### 7.3  Encadenar algoritmos (opcional)

1. Cifras con XOR → produce `foto.png.xor`.
2. Vuelves a ejecutar y elijes Vigenère sobre **ese mismo** archivo: resultado `foto.png.xor.vig`.
3. Para regresar al original revierte **en orden inverso**.

---

## 8. Solución de Problemas<a name="solucion-de-problemas"></a>

| Síntoma                    | Causa frecuente                    | Solución                                                                                |
| -------------------------- | ---------------------------------- | --------------------------------------------------------------------------------------- |
| `Bytes escritos: 0`        | Archivo vacío o clave equivocada.  | Verifica que `raw/archivo` no pese 0 B y que introduces la misma clave en decrypt.      |
| `No se pudo abrir <ruta>`  | Ruta mal escrita o permisos.       | Usa nombre correcto y revisa que el archivo no esté abierto en otro programa (Windows). |
| `Validación interna falló` | Implementación del algoritmo rota. | Revisa modificaciones recientes en las clases de cifrado.                               |

---

## 9. Seguridad y Buenas Prácticas<a name="seguridad"></a>

* **Destruye** los archivos temporales cifrados si contienen datos sensibles.
* No uses la clave DES fija para producción ⚠️.
* Usa claves largas y únicas en XOR/Vigenère.
* Ejecuta el binario en discos locales, no en ubicaciones sincronizadas (OneDrive) para evitar escrituras parciales.

---

## 10. Extensiones Futuras<a name="extensiones-futuras"></a>

* **AES‑256 GCM** con clave y IV aleatorios.
* Soporte **ARGON2** para derivación de claves.
* GUI Qt (aprovechando tu proyecto paralelo).
* Hash y firma digital (SHA‑256 + RSA).

---

## 11. Licencia<a name="licencia"></a>

`Criptobro CLI` se distribuye bajo la licencia MIT. Consulta `LICENSE` para más detalles.

---

> **¡Listo!** Con este README tienes todo lo necesario para compilar, ejecutar y extender tu herramienta de cifrado.
