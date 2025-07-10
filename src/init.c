
#include "init.h"

/**
 * @brief Inicializa o barramento I2C1 para uso com o display.
 */
void initDisplayI2C() {
    // Inicializa a instância i2c1 com velocidade de 400kHz.
    i2c_init(i2c1, 400 * 1000);
    
    // Define os pinos GPIO para as funções I2C, com base nos valores em display.h.
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
    // Ativa os resistores de pull-up internos.
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

/**
 * @brief Inicializa todos os subsistemas necessários para a aplicação.
 */
void initializeSystem(void) {
    // Inicializa a comunicação serial (USB/UART) para a função printf.
    stdio_init_all();
    sleep_ms(1000); // Delay para permitir a conexão do monitor serial.

    // Configura o barramento I2C para o display OLED.
    printf("Configurando I2C do OLED...\n");
    initDisplayI2C();

    // Inicializa o controlador do display.
    printf("Iniciando SSD1306...\n");
    if (initializeDisplay() != 0) {
        printf("Erro ao inicializar o SSD1306\n");
        // O programa poderia travar aqui em um sistema real para indicar falha crítica.
    }
 
    // Limpa a tela para garantir que ela esteja vazia no início.
    printf("Tela limpa.\n");
    clearScreen();
}