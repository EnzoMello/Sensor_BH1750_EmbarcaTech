
#include "display.h"

ssd1306_t display;

/**
 * @brief Inicializa o display SSD1306.
 * @return 0 em caso de sucesso, 1 em caso de falha.
 */
int initializeDisplay() {
    // Chama a função de inicialização da biblioteca ssd1306, passando os parâmetros definidos.
    if (!ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, i2c1)) {
        printf("Falha ao inicializar o display SSD1306\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Exibe um texto na tela em uma posição e escala específicas.
 * @param texto A string de texto a ser exibida.
 * @param x A coordenada X (horizontal) inicial do texto.
 * @param y A coordenada Y (vertical) inicial do texto.
 * @param scale O fator de escala da fonte.
 */
void showText(const char *texto, uint32_t x, uint32_t y, uint32_t scale){
    ssd1306_draw_string(&display, x, y, scale, texto); // Desenha o texto no buffer.
    ssd1306_show(&display); // Envia o buffer para a tela.
}

/**
 * @brief Limpa completamente a tela do display.
 */
void clearScreen(){
    ssd1306_clear(&display); // Limpa o buffer.
    ssd1306_show(&display); // Envia a tela limpa.
}

/**
 * @brief Atualiza uma linha de texto, limpando a área antes de desenhar.
 * @param text A nova string de texto a ser exibida.
 * @param x A coordenada X (horizontal) inicial do texto.
 * @param y A coordenada Y (vertical) inicial do texto.
 * @param scale O fator de escala da fonte.
 * @param clear_width A largura da área (em pixels) a ser limpa.
 */
void updateTextLine(const char* text, uint32_t x, uint32_t y, uint32_t scale, uint32_t clear_width) {
    // A altura da fonte padrão na biblioteca é 8 pixels.
    uint32_t char_height = 8;

    // Limpa uma área retangular onde o novo texto será escrito.
    ssd1306_clear_square(&display, x, y, clear_width, char_height * scale);

    // Desenha a nova string no buffer.
    ssd1306_draw_string(&display, x, y, scale, text);
}