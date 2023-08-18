# Tic Tac Toe

Jogo da velha 3x3 e 4x4 contra o PC usando minimax com memorização
(guarda todos os estados alcançáveis em uma tabela juntamente com o melhor resultado para o jogador) 
e prunning (reverte linhas, colunas e transpõe o tabuleiro, vê se o jogador pode ganhar em uma ou duas jogadas, se deve colocar a peça numa determinada posição para não perder e também alfa-beta prunning).

![inicio](https://github.com/ThiagoFBastos/tic_tae_toe/blob/main/data/start.png) <br>

![3x3](https://github.com/ThiagoFBastos/tic_tae_toe/blob/main/data/3x3-9.png) <br>

![4x4](https://github.com/ThiagoFBastos/tic_tae_toe/blob/main/data/4x4-16.png) <br>

## Requisitos
-  g++ que suporta c++17
-  SDL2-devel
-  SDL2_image-devel
-  SDL2_ttf-devel
-  GNU C++ PBDS
-  GNU Make

## Instalação
1. Abra o terminal e use o comando make

## Instruções
1. Escolha o tamanho do tabuleiro 3x3 ou 4x4
2. Na sua vez escolha uma célula vazia e clique que ela será preenchida com vermelho
3. O PC usa a cor azul para preencher as células
