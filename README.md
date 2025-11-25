# erro404

## **Descrição:**

  “Erro: 404” é uma experiência narrativa sobre a memória, o abandono e o silêncio digital.

  O jogador explora um mundo incompleto — um jogo que parece ter sido esquecido por seu próprio criador.

  À medida que avança, descobre ambientes inacabados, personagens que repetem falas quebradas e arquivos de desenvolvimento deixados para trás.

  No fim, a verdade emerge: o jogo foi interrompido por um motivo trágico, e o protagonista passa a compreender que ele próprio é parte desse projeto inacabado — um fragmento da mente de alguém que não está mais aqui.

  Jogo narrativo que explora glitches, memórias corrompidas e a sensação de estar perdido num jogo esquecido.

## **Dependencias**:

  ### Linux:
  - gcc
  - make
  - raylib

  - #### instalação das dependencias:

    #### Arch: 
    ```bash
    sudo pacman -S gcc make raylib
    ```
        
    #### Ubuntu:
    ```bash
    sudo apt install gcc make libraylib-dev
    ```

  ### Windows:
  - [wdevkit](https://github.com/skeeto/w64devkit/releases)

  - #### instalação das dependencias:
    Abra o repositorio oficial do [wdevkit](https://github.com/skeeto/w64devkit/releases) e baixe o exe correspondente.
      
## **Como compilar:**

  ### Linux:

  1. Clone o repositorio do [raylib](https://github.com/raysan5/raylib):

  ```bash
  git clone https://github.com/raysan5/raylib.git
  ```

  2. Rode make na pasta src dentro do **raylib**: `raylib/raylib/src`

  ```bash
  make PLATFORM=PLATFORM_DESKTOP 
  ```

  3. copie o arquivo criado, `libraly.a`

  4. Clone o repositorio do projeto:

  ```bash
  git clone https://github.com/Natan-Tavares/erro404.git
  ```

  5. Abra a pasta `lib` na pasta do projeto e cole o arquivo `libraly.a`

  6. Na pasta raiz do projeto faça o `make`:
    
  ```bash
  make
  ```

  7. Rode o projeto:

  ```bash
  ./main
  ```
  ### Windows:

  1. Baixe o [raylib](https://www.raylib.com/) do site oficial.

  2. Abra a pasta `src` dentro da pasta do `raylib`: `raylib/raylib/src`.

  3. Copie o arquivo `libraly.a`da pasta src.

  4. Clone o repositorio do projeto: 
  ```bash
  git clone https://github.com/Natan-Tavares/erro404.git
  ```

  5. Abra a pasta `lib` na pasta do projeto e cole o arquivo `libraly.a`.

  6. Na pasta raiz do projeto, compile-o:
  ```bash
  make
  ```

  7. Rode o projeto:
  ```bash
  main.exe
  ```