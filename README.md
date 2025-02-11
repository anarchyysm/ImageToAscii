# ImageToAscii

ImageToAscii é um programa escrito em C++ que converte imagens e vídeos em representações ASCII art. O programa usa OpenCV para manipulação de imagens e CMake para gerenciamento de dependências.

## Funcionalidades
- Conversão de imagens para ASCII art
- Conversão de vídeos para ASCII art quadro a quadro
- Suporte a diferentes resoluções e ajustes de brilho
- Configuração personalizável de caracteres ASCII

## Tecnologias Utilizadas
- **Linguagem:** C++
- **Bibliotecas:** OpenCV
- **Gerenciador de Build:** CMake

## Como Compilar e Executar
### Requisitos
- CMake
- OpenCV instalado
- Compilador C++ (GCC, Clang, MSVC, etc.)

### Compilando o projeto
```sh
 cmake --build cmake-build-debug --target ConvertImageToAscii -j 10

```

### Executando o programa

(precisa adicionar imagem na linha de codigo dentro de /cmake-build-debug antes de compilar *temporario*)
```sh
./ImageToAscii
```

## Exemplo:
### Saída: 
![Foto de exemplo](example.png)

## Licença
Este projeto está licenciado sob a [MIT License](LICENSE).


