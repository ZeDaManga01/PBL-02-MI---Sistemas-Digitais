
<h1 align="center"> Barramento </h1>



## Contexto do Projeto
A evolução da tecnologia de processamento gráfico tem permitido avanços significativos na manipulação e controle de elementos visuais em diversos dispositivos. Neste contexto, destaca-se o trabalho do discente Gabriel Sá Barreto Alves, que, em suas pesquisas de iniciação científica e no desenvolvimento de seu trabalho de conclusão de curso, projetou e implementou um Processador Gráfico capaz de mover e controlar elementos em um monitor VGA com resolução de 640×480 pixels. Este processador é capaz de desenhar polígonos convexos, como quadrados e triângulos, além de gerenciar uma quantidade predefinida de sprites, utilizando o NIOS II como unidade de processamento principal.
No entanto, a introdução da plataforma DE1-SoC introduz a dispensabilidade do uso do NIOS II, uma vez que inclui um processador ARM (HPS) capaz de enviar as instruções necessárias para o Processador Gráfico. Por isso, o cerne do problema reside na comunicação entre o HPS e o Processador Gráfico na FPGA. A solução proposta envolve o desenvolvimento de módulos de kernel no HPS que permitam essa interação, bem como a criação de uma biblioteca em C para simplificar o processo de programação.


## Requisitos


- Aplicar conhecimentos de interação hardware-software;
- Compreender o mapeamento de memória em uma arquitetura ARM;
- Utilizar a interface de conexão entre HPS e FPGA da DE1-SoC;
- Programar em C para um processador com arquitetura ARM;
- Entender políticas de gerenciamento de sistema operacional Linux em arquitetura ARM;
- Compreender os princípios básicos da arquitetura da plataforma DE1-SoC.

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
        <li><a href="#resultado"> Resultados alcançados</a></li>
        <li><a href="#conclusion"> Conclusão</a></li>
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

### Portas USB 2.0

A placa possui duas portas USB 2.0 tipo A com um controlador SMSC USB3300 e um hub de 2 portas
controlador. O dispositivo SMSC USB3300 em pacote QFN de 32 pinos faz interface com o SMSC
Controlador de hub USB2512B.

<div id="metodo"> 
<h2> Metodologia </h2>
<div align="justify">
  
### Driver

### Biblioteca

### Comunicação Driver/Biblioteca




<div id="test"> 
<h2> Testes</h2>
<div align="justify">


Esta sessão é reservada para demonstração dos testes feitos no projeto.


<p align="center">
  <img src="" />
</p>

  <h4 align="center"> Primeiro Momento </h4>


<p align="center">
  <img src="" />
</p>

  <h4 align="center"> Segundo Momento </h4>


<p align="center">
  <img src="" />
</p>

</p>
 <h4 align="center"> Momento Final </h4>


<div id="resultado"> 
<h2> Resultados alcançados</h2>
<div align="justify">

<div id="conclusion"> 
<h2> Conclusão</h2>
<div align="justify">


<div id="makefile"> 
<h2> Instruções para executar o programa</h2>
<div align="justify">


```
make
sudo ./ttt
```


<div id="ref"> 
<h2> Referências </h2>
<div align="justify">
  
DE1-SoC Board. Disponível em: https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&No=836&PartNo=4. Acessado em: 7 de maio de 2024.

Introduction to the ARM Cortex-A9 Processor. Disponível em: https://github.com/fpgacademy/Tutorials/releases/download/v21.1/ARM_intro_intelfpga.pdf. Acessado em: 5 de maio de 2024.


