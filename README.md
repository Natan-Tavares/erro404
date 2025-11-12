# erro404
(PLACE HOLDER)
## Descrição
“Erro: 404” é uma experiência narrativa sobre a memória, o abandono e o silêncio digital.

O jogador explora um mundo incompleto — um jogo que parece ter sido esquecido por seu próprio criador.

À medida que avança, descobre ambientes inacabados, personagens que repetem falas quebradas e arquivos de desenvolvimento deixados para trás.

No fim, a verdade emerge: o jogo foi interrompido por um motivo trágico, e o protagonista passa a compreender que ele próprio é parte desse projeto inacabado — um fragmento da mente de alguém que não está mais aqui.

Jogo narrativo que explora glitches, memórias corrompidas e a sensação de estar perdido num jogo esquecido.

---
#### Como compilar:

1. Baixe o setup do [raylib](https://www.raylib.com/) no site oficial e baixe no seu computador.

2. No local de instalação vá ate `instalação/raylib/w64devkit/bin`, copie o caminho e adicione no seu PATH do windows.

3. No local de instalação vá ate `instalação/raylib/raylib/src`, então rode:

```bash
 make PLATFORM=PLATFORM_DESKTOP 
 ```

4. Copie o arquivo `libraylib.a` para dentro da pasta lib do projeto

5. Na pasta raiz do projeto rode `make`, e então você tera compilado o projeto para um arquivo main.exe
