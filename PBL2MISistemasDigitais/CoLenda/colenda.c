#include "colenda.h"

int gpu_open(FILE **file, const char *driver_name)
{
    *file = fopen(driver_name, "r+");

    if (!(*file)) {
        return -EOPEN;
    }

    return SUCCESS;
}

int gpu_close(FILE **file)
{
    if(!fclose(*file)) {
        return -ECLOSE;
    }

    *file = NULL;

    return SUCCESS;
}

int gpu_write(FILE *file, const char *buffer)
{
    int bytes_write = fwrite(buffer, sizeof(char), BUFFER_SIZE, file);

    fseek(file, 0, SEEK_SET);
		
    if (bytes_write != 8) {
        return -EWRITE;
    }

    return SUCCESS;
}

int copy_to_buffer(char *buffer, size_t size, uint64_t instruction) {
    if (size < BUFFER_SIZE) {
        return -ESIZE;
    }

    for (int i = sizeof(instruction) - 1, j = 0; i >= 0; i--, j++) {
        buffer[i] = (instruction >> j * 8);
    }

    return SUCCESS;
}

/**
 * @brief Define a cor de fundo do jogo.
 *
 * Esta função gera uma instrução uint64_t que representa um comando para enviar ao driver de caracteres.
 * A instrução inclui o opcode (WBR) e os valores RGB da cor. Os valores RGB são empacotados na instrução
 * usando operações bit a bit. A instrução é então dividida em blocos de 8 bits e armazenada no buffer fornecido.
 *
 * @param gpu Um ponteiro para o arquivo da GPU.
 * @param red O componente vermelho da cor (0-7).
 * @param green O componente verde da cor (0-7).
 * @param blue O componente azul da cor (0-7).
 *
 * @return SUCCESS se a operação for bem-sucedida, EINPUT se os valores de entrada forem inválidos,
 * ESIZE se o buffer não tiver espaço suficiente para armazenar a instrução,
 * EWRITE se ocorrer um erro de escrita no arquivo.
 */
int set_background(FILE *gpu, uint8_t red, uint8_t green, uint8_t blue)
{
    char buffer[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};

    if ((red | green | blue) & ~0x7) /* Entrada do usuário inválida */ {
        return -EINPUT;
    }

    /* dataA*/
    uint64_t instruction = WBR; // Atribuindo o opcode da instrução

    /* dataB */
    instruction |= (uint64_t) (red & 0x7) << 32; // Início da área reservada para o segundo registrador
    instruction |= (uint64_t) (green & 0x7) << 35; // 3 bits da cor passada
    instruction |= (uint64_t) (blue & 0x7) << 38; // 6 bits

    if (copy_to_buffer(buffer, BUFFER_SIZE, instruction) != 0) {
        return -ESIZE;
    }

    return gpu_write(gpu, buffer);
}

/**
 * @brief Define os parâmetros do sprite no buffer de instruções.
 *
 * Esta função gera uma instrução de 64 bits que representa um comando para enviar ao driver de caracteres.
 * A instrução inclui o opcode (WBR), registrador do sprite, sinalizador de visibilidade, coordenadas e deslocamento.
 * Os parâmetros são empacotados na instrução usando operações bit a bit. A instrução é então dividida em
 * blocos de 8 bits e armazenada no buffer fornecido.
 *
 * @param gpu Um ponteiro para o arquivo da GPU.
 * @param layer O registrador do sprite (0-31).
 * @param show O sinalizador de visibilidade (0: oculto, 1: visível).
 * @param x A coordenada x do sprite (0-1023).
 * @param y A coordenada y do sprite (0-1023).
 * @param sprite O deslocamento do sprite na memória (0-511).
 *
 * @return SUCCESS se a operação for bem-sucedida, EINPUT se os valores de entrada forem inválidos,
 * ESIZE se o buffer não tiver espaço suficiente para armazenar a instrução,
 * EWRITE se ocorrer um erro de escrita no arquivo.
 */
int set_sprite(FILE *gpu, uint8_t layer, int show, uint16_t x, uint16_t y, uint16_t sprite)
{
    char buffer[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};

    if (((x | y) & ~0x3FF) || (sprite & ~0x1FF) || (layer & ~0x1F)) /* Entrada do usuário inválida */ {
        return -EINPUT;
    }

    uint64_t instruction = WBR;

    // dataA
    instruction |= (uint64_t) (layer & 0x1F) << 4;
    
    // dataB
    instruction |= (uint64_t) (sprite & 0x1FF) << 32;
    instruction |= (uint64_t) (y & 0x3FF) << 41;
    instruction |= (uint64_t) (x & 0x3FF) << 51;
    instruction |= (uint64_t) (show & 0x1) << 61;
    
    copy_to_buffer(buffer, BUFFER_SIZE, instruction);

    if (copy_to_buffer(buffer, BUFFER_SIZE, instruction) != 0) {
        return -ESIZE;
    }

    return gpu_write(gpu, buffer);
};

/**
 * @brief Define os parâmetros de um polígono no buffer de instruções.
 *
 * Esta função gera uma instrução de 64 bits que representa um comando para enviar ao driver de caracteres.
 * A instrução inclui o opcode (DP), endereço, tipo, coordenadas, cor e tamanho.
 * Os parâmetros são empacotados na instrução usando operações bit a bit. A instrução é então dividida em
 * blocos de 8 bits e armazenada no buffer fornecido.
 *
 * @param gpu Um ponteiro para o arquivo da GPU.
 * @param layer O endereço do polígono (0-15).
 * @param type O tipo do polígono (0: quadrado, 1: triângulo).
 * @param x A coordenada x do polígono (0-511).
 * @param y A coordenada y do polígono (0-511).
 * @param red O componente vermelho da cor (0-7).
 * @param green O componente verde da cor (0-7).
 * @param blue O componente azul da cor (0-7).
 * @param size O tamanho do polígono (0-15).
 *
 * @return SUCCESS se a operação for bem-sucedida, EINPUT se os valores de entrada forem inválidos,
 * ESIZE se o buffer não tiver espaço suficiente para armazenar a instrução,
 * EWRITE se ocorrer um erro de escrita no arquivo.
 */
int set_poligon(FILE *gpu, uint_fast8_t layer, uint_fast8_t type, uint16_t x, uint16_t y, uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue, uint_fast8_t size)
{
    char buffer[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};

    if (((red | green | blue) & ~0x7) || (size & ~0xF) || ((x | y) & ~0x1FF) || (type & ~0x1) || (layer & ~0x1F)) /* Entrada do usuário inválida */ {
        return -EINPUT;
    }

    uint64_t instruction = DP;

    /* dataA */
    instruction |= (uint64_t) (layer & 0x1F) << 4;

    /* dataB */
    instruction |= (uint64_t) (x & 0x1FF) << 32;
    instruction |= (uint64_t) (y & 0x1FF) << 41;
    instruction |= (uint64_t) (size & 0xF) << 50;
    instruction |= (uint64_t) (red & 0x7) << 54;
    instruction |= (uint64_t) (green & 0x7) << 57;
    instruction |= (uint64_t) (blue & 0x7) << 60;
    instruction |= (uint64_t) (type & 0x1) << 63;

    if (copy_to_buffer(buffer, BUFFER_SIZE, instruction) != 0) {
        return -ESIZE;
    }

    return gpu_write(gpu, buffer);
};

/**
 * @brief Define a cor de um bloco específico no background.
 *
 * Esta função calcula o endereço do bloco com base nas linhas e colunas fornecidas,
 * e então gera uma instrução WBM para escrever os dados de cor no endereço especificado na memória de background.
 * A função verifica os valores de entrada válidos e retorna um código de erro se necessário.
 *
 * @param gpu Um ponteiro para o arquivo da GPU.
 * @param row A linha do índice do bloco (baseado em 0).
 * @param column A coluna do índice do bloco (baseado em 0).
 * @param red O componente vermelho da cor (0-7).
 * @param green O componente verde da cor (0-7).
 * @param blue O componente azul da cor (0-7).
 *
 * @return SUCCESS se a operação for bem-sucedida, EINPUT se os valores de entrada forem inválidos,
 * ESIZE se o buffer não tiver espaço suficiente para armazenar a instrução.
 * EWRITE se ocorrer um erro de escrita no arquivo.
 */
int set_background_block(FILE *gpu, uint_fast8_t row, uint_fast8_t column, uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue)
{
    char buffer[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};

    uint16_t address = row * (SCREEN_WIDTH / BLOCK_SIZE) + column;

    if ((red | green | blue) & ~0x7 || (address & ~0x3FFF) >= ((SCREEN_HEIGHT / BLOCK_SIZE) * (SCREEN_WIDTH / BLOCK_SIZE))) /* Entrada do usuário inválida */ {
        return -EINPUT;
    }

    uint64_t instruction = WBM;

    /* dataA */
    instruction |= (uint64_t) (address & 0x3FFF) << 4;

    /* dataB */
    instruction |= (uint64_t) (red & 0x7) << 32; // Início da área reservada para o segundo registrador
    instruction |= (uint64_t) (green & 0x7) << 35; // 3 bits da cor passada
    instruction |= (uint64_t) (blue & 0x7) << 38; // 6 bits

    if (copy_to_buffer(buffer, BUFFER_SIZE, instruction) != 0) {
        return -ESIZE;
    }

    return gpu_write(gpu, buffer);
};

/**
 * @brief Imprime o conteúdo de um buffer em formato hexadecimal.
 *
 * Esta função itera pelos elementos de um buffer e imprime cada byte em formato hexadecimal.
 * É útil para depuração e inspecionar o conteúdo de um buffer.
 *
 * @param buffer Um ponteiro para o buffer cujo conteúdo será impresso.
 * @param size O número de elementos no buffer.
 *
 * @return void
 */
void print_buffer(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", (unsigned char) buffer[i]);
    }
}
