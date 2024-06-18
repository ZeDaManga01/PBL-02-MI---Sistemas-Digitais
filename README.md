
<h1 align="center"> Barramento </h1>



## Contexto do Projeto
A evolução da tecnologia de processamento gráfico tem permitido avanços significativos na manipulação e controle de elementos visuais em diversos dispositivos. Neste contexto, destaca-se o trabalho do discente Gabriel Sá Barreto Alves, que, em suas pesquisas de iniciação científica e no desenvolvimento de seu trabalho de conclusão de curso, projetou e implementou um Processador Gráfico capaz de mover e controlar elementos em um monitor VGA com resolução de 640×480 pixels. Este processador é capaz de desenhar polígonos convexos, como quadrados e triângulos, além de gerenciar uma quantidade predefinida de sprites, utilizando o NIOS II como unidade de processamento principal.

No entanto, a introdução da plataforma DE1-SoC introduz a dispensabilidade do uso do NIOS II, uma vez que inclui um processador ARM (HPS) capaz de comunicar as instruções necessárias para o Processador Gráfico. Por conta disso, a ausência de um meio de comunicação entre o HPS e o Processador Gráfico na FPGA consta como um imbróglio para a sua utilização no Kit de Desenvolvimento. Destarte, a solução proposta descrita neste documento envolve a descrição do desenvolvimento de módulos de kernel no HPS que permitam essa interação, bem como a criação de uma biblioteca em C para simplificar o processo de programação. O projeto foi desenvolvido em cima da arquitetura desenvolvida pelo discente, acessando os componentes do Processador Gráfico através de um mapeamento de memória.


## Requisitos

A solução para o problema a ser desenvolvida no Kit de desenvolvimento DE1-SoC deve atender às seguintes restrições:

- O desenvolvimento de módulos de kernel para implementar a comunição do HPS com o Processador Gráfico.
- O desenvolvimento de uma biblioteca responsável por sintetizar as instruções e se comunicar com o módulo de kernel desenvolvido.
- O código deve ser escrito em linguagem C;
- A biblioteca deve conter no mínimo uma função para cada Instrução do Processador Gráfico.

Ademais, uma imagem deve ser renderizada, com a utilização da biblioteca desenvolvida, como resultado do produto.

## Equipe

<uL>
  <li><a href="https://github.com/pierremachado">Pierre Machado</a></li>
  <li><a href="https://github.com/kevincorges">Kevin Borges</a></li>
  <li><a href="https://github.com/ZeDaManga01">José Alberto</a></li>
</ul>

<h1 align="center"> Sumário </h1>
<div id="sumario">
        <ul>
        <li><a href="#sft_ut"> Softwares Utilizados </a></li>
        <li><a href="#kit_placa"> Kit de Desenvolvimento DE1-SoC</a></li>
        <li><a href="#comp_u">  Componentes Utilizados</a></li>
        <li><a href="#metodo"> Metodologia</a></li>
        <li><a href="#test"> Testes</a></li>
        <li><a href="#resultconcl"> Resultados e conclusão</a></li>
        <li><a href="#makefile"> Instruções de execução</a></li>
        <li><a href="#ref"> Referências</a></li>
        </ul>
</div>

<div id="sft_ut"> 
<h2> Softwares Utilizados</h2>
<div align="justify">

### GNU/Linux

Por fim, o kit de desenvolvimento DE1-SoC possui uma distribuição do Linux embarcado instalada, possibilitando a comunicação com o kit bem como a execução dos códigos criados através de conexão remota. Isso oferece uma gama de possibilidades para a elaboração do problema: a disposição dos diretórios do sistema e a possibilidade de compilar códigos na linguagem de programação requisitada de forma fácil com o compilador gcc embarcado no kit de desenvolvimento foram fundamentais.

### VS Code

O Visual Studio Code (VS Code) é uma ferramenta popular e altamente funcional utilizada em uma variedade de projetos de desenvolvimento de software. O uso dele foi necessário para agilizar o desenvolvimento, permitindo editar, depurar e gerenciar o código de maneira simplificada e eficaz.

### C

A linguagem C é amplamente utilizada em projetos devido à sua eficiência e versatilidade. Com sua sintaxe direta e controle próximo sobre o hardware, o C permite desenvolver programas robustos e rápidos, especialmente em sistemas embarcados, drivers de dispositivos e software de baixo nível. No contexto deste projeto, a utilização da linguagem C foi um dos requisitos necessarios a serem cumpridos.

<div id="kit_placa"> 
<h2> Kit de Desenvolvimento DE1-SoC</h2>
<div align="justify">

O kit de desenvolvimento DE1-SoC apresenta uma plataforma robusta de design de hardware construída em torno do Altera
FPGA System-on-Chip (SoC), que combina os mais recentes núcleos incorporados Cortex-A9 dual-core
com lógica programável líder do setor para máxima flexibilidade de projeto.

<p align="center">
  <img src="https://github.com/ZeDaManga01/PBL-01-MI---Sistemas-Digitais/blob/main/docs/image%20-%20De1-SoC.jpeg" />
</p>

### ARM CORTEX A9

A arquitetura utilizada por esse processador é a RISC – Reduced Instruction Set
Computer, suas operações lógicas e aritméticas são efetuadas em operadores dos registradores
de propósitos gerais. Os dados são movidos entre a memória e esses registradores por meio de
instruções de carga e armazenamento - Load and Store. 

### Registradores

O ARM possui 16 registradores de 32 bits, sendo 15 de uso geral, R0 a R14 e um
Program Counter R15. O registrador R15 tem o endereço da próxima instrução que será
executada. Os registradores R13 e R14, são usados convencionalmente como ponteiro de
pilha Stack Pointer, que contém o endereço atual do elemento superior da pilha e registrador
de link Link Register, que recebe o endereço de retorno em chamadas de procedimento,
respectivamente.

### Memória

A placa suporta 1GB de SDRAM DDR3, compreendendo dois dispositivos DDR3 de 16 bits cada no lado do HPS.
Os sinais estão conectados ao Controlador de Memória Dedicado para os bancos de I/O do HPS
e a velocidade alvo é de 400 MHz.

### Diagrama de Blocos da DE1-SoC

Todas as conexões são estabelecidas através do
Dispositivo Cyclone V SoC FPGA para fornecer flexibilidade máxima aos usuários. Os usuários podem configurar o
FPGA para implementar qualquer projeto de sistema.

<p align="center">
  <img src="https://github.com/ZeDaManga01/PBL-01-MI---Sistemas-Digitais/blob/main/docs/Image.jpeg" />
</p>

<div id="comp_u"> 
<h2> Componentes Utilizados</h2>
<div align="justify">

### Gigabit Ethernet
 
A placa suporta transferência Gigabit Ethernet por um chip externo Micrel KSZ9021RN PHY e
Função HPS Ethernet MAC. O chip KSZ9021RN com Gigabit 10/100/1000 Mbps integrado
O transceptor Ethernet também suporta interface RGMII MAC.

### Monitor CRT

O monitor utilizado no projeto foi o DELL M782p, um modelo CRT. Esse tipo de monitor utiliza um tubo de raios catódicos (CRT) para exibir imagens. O DELL M782p possui uma tela de visualização de 17 polegadas e uma resolução máxima de 1280x1024 pixels. Ele oferece uma interface VGA para conexão com o computador ou placa de desenvolvimento. Os monitores CRT são conhecidos por sua reprodução de cores vibrantes e tempos de resposta rápidos, tornando-os uma escolha adequada para projetos que exigem interação em tempo real, como jogos e simulações.

### GPU

Unidade de processamento gráfico, também conhecida como GPU, é um componente eletrônico projetado para acelerar tarefas relacionadas à computação gráfica e ao processamento de imagens em uma ampla gama de dispositivos, incluindo placas de vídeo, placas-mãe, smartphones e computadores pessoais (PCs). A capacidade da GPU de realizar cálculos matemáticos complexos de forma rápida e eficiente reduz significativamente o tempo necessário para que um computador execute uma variedade de programas

<div id="metodo"> 
<h2> Metodologia </h2>
<div align="justify">

Essa seção visa descrever o processo de elaboração do módulo de kernel e a sua biblioteca adjacente.
  
### Módulo de Kernel

De acordo com o livro Sistemas Operacionais Modernos, "o núcleo encontra-se diretamente sobre o hardware e possibilita interações com os dispositivos de E/S e a unidade de gerenciamento de memória, e controla o acesso da CPU a eles" [TANENBAUM & BOS, 2008]. O kernel (ou núcleo) do sistema operacional atua em uma camada intermediária entre o software e o hardware, implementando a comunicação entre eles e sendo responsável por gerenciar todo o sistema.

Para que o sistema operacional identifique corretamente os componentes físicos, o kernel carrega e gerencia drivers (módulos) de dispositivos. Esses drivers são programas específicos para cada componente de hardware e são essenciais para que o sistema operacional se comunique corretamente com o hardware e esteja no controle de chamadas do sistema de programas para esses dispositivos. Devido a isso, foi suscitada a necessidade de desenvolvimento de um módulo kernel de caracteres para implementar a comunicação do HPS do DE1-SoC com o Processador Gráfico.

Ao ser inicializado e alocado na memória, o módulo de kernel desenvolvido para o projeto realiza o mapeamento de memória (MMIO) dos componentes do Processador Gráfico, sendo estes:
- Registrador DATA_A: registrador do Processador Gráfico responsável por receber a instrução e o endereço de memória, caso necessário;
- Registrador DATA_B: registrador do Processador Gráfico responsável por receber as configurações da instrução (e.g., cores, posição, tamanho, entre outros);
- Registrador WRREG: registrador do Processador Gráfico responsável por receber um bit que permite a adição da instrução à FIFO de instruções;
- Registrador WRFULL: registrador do Processador Gráfico que recebe um bit 1 caso a FIFO de instruções esteja cheia e um bit 0 caso tenha espaço. Assim, para garantir que as instruções sejam executadas corretamente, o driver desenvolvido não envia a próxima instrução caso a FIFO esteja cheia.

Dessa forma, o sistema interpreta a escrita no arquivo nó, localizado no diretório /dev, como uma chamada de escrita do módulo. Então, o módulo recebe um buffer de 8 caracteres (8 bytes, 1 byte para cada char) do espaço de usuário e, utilizando a técnica de deslocamento de bits, separa os primeiros 4 bytes para o registrador DATA_A e os 4 bytes restantes para o registrador DATA_B.
O driver possui checagens de erro, como, por exemplo, a verificação do tamanho do buffer de instrução recebido. Caso o buffer recebido tenha menos de 8 caracteres, o comando não é executado. Além disso, caso haja falha durante a alocação do driver na memória, o driver é automaticamente desalocado e uma mensagem de depuração é lançada indicando o problema.

### Biblioteca

Visando facilitar o entendimento e a correta utilização do driver desenvolvido, uma biblioteca adjacente também foi desenvolvida. A biblioteca possui quatro funções:

- Definir o plano de fundo;
- Definir um polígono, recebendo como parâmetros: posição horizontal e vertical, cor, tamanho, camada e tipo de polígono convexo (quadrado/triângulo);
- Definir um sprite, recebendo como parâmetros: endereço, camada do sprite, posição e valor booleano para mostrar ou ocultar o sprite na camada designada;
- Definir um bloco de plano de fundo, recebendo como parâmetros: linha, coluna e cor desejada.
  
As funções validam as entradas do usuário e retornam resultados de erro caso necessário. Através da técnica de deslocamento de bits, a biblioteca utiliza constantes para identificar a instrução e formata as entradas do usuário em um buffer de 8 caracteres, enviando-o para o driver através de uma chamada de escrita no arquivo nó, localizado no diretório /dev.

<div id="test"> 
<h2> Testes</h2>
<div align="justify">

Esta sessão é reservada para demonstração dos testes feitos no projeto.

  <h4 align="center"> Primeiro Momento </h4>
<p align="center">
  <img src="https://github.com/ZeDaManga01/M-dias---PBL2---MI-_SD/blob/main/1st%20Moment.jpeg" />
</p>
  

  <h4 align="center"> Segundo Momento </h4>
<p align="center">
  <img src="https://github.com/ZeDaManga01/M-dias---PBL2---MI-_SD/blob/main/2nd%20Moment.jpeg" />
</p>
  

  <h4 align="center"> Momento Final </h4>
<p align="center">
  <img src="https://github.com/ZeDaManga01/M-dias---PBL2---MI-_SD/blob/main/Final%20Moment.jpeg" />
</p>
 

<div id="resultconcl"> 
<h2> Resultados e Conclusão</h2>
<div align="justify">

Conclui-se que o projeto foi implementado de forma satisfatória. O driver e a biblioteca se mostraram funcionais e atenderam aos requisitos propostos. Ademais, foi essencial para expandir o conhecimento acerca do kit de desenvolvimento, GNU/Linux embarcado e a comunicação hardware/software, contribuindo para a sofisticação de projetos futuros a serem implementados no kit de desenvolvimento DE1-SoC.


<div id="makefile"> 
<h2> Instruções para executar o programa</h2>
<div align="justify">

Abrindo a pasta do projeto no terminal, escreva:

```
cd CoLendaDriver
sudo make install
```

Siga as instruções mostradas no fim da instalação do driver para a criação do arquivo nó.

Então, volte para o diretório do projeto, e escreva:

```
make
sudo ./main
```

Para desinstalar o driver, com o diretório do projeto aberto no terminal, escreva:

```
cd CoLendaDriver
sudo make uninstall
```

<div id="ref"> 
<h2> Referências </h2>
<div align="justify">
  
DE1-SoC Board. Disponível em: https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=836&PartNo=4. Acessado em: 7 de maio de 2024.

Introduction to the ARM Cortex-A9 Processor. Disponível em: https://github.com/fpgacademy/Tutorials/releases/download/v21.1/ARM_intro_intelfpga.pdf. Acessado em: 5 de maio de 2024.

TANENBAUM, Andrew S. Sistemas Operacionais Modernos. 4. ed. São Paulo: Pearson Prentice Hall, 2008. p. 504.


