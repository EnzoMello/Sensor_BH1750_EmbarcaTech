
#include "bh1750.h"

// Constantes do BH1750 
const uint8_t BH1750_ADDR = 0x23; ///< Endereço I2C padrão do sensor BH1750.
const uint8_t BH1750_POWER_ON = 0x01; ///< Comando para ligar o sensor.
const uint8_t BH1750_CONT_HIGH_RES_MODE = 0x10; ///< Comando para o modo de medição contínua de alta resolução (1 lux).

/**
 * @brief Inicializa o barramento I2C e o sensor BH1750.
 * @param i2c_instance A instância I2C a ser usada (ex: i2c0, i2c1).
 * @param sda_pin O pino GPIO para a linha SDA.
 * @param scl_pin O pino GPIO para a linha SCL.
 * @return Retorna um ponteiro para a instância I2C inicializada para uso futuro.
 */
i2c_inst_t* bh1750_init(i2c_inst_t *i2c_instance, uint sda_pin, uint scl_pin) {
    printf("Configurando BH1750 no I2C...\n");

    // 1. Inicializa o barramento I2C com a instância e velocidade desejada (100kHz).
    i2c_init(i2c_instance, 100 * 1000);
    
    // 2. Define os pinos GPIO para as funções I2C.
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    
    // 3. Ativa as resistências de pull-up internas para estabilidade do barramento.
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    
    // 4. Configura o sensor BH1750 ligando-o e definindo o modo de operação.
    bh1750_set_mode(i2c_instance, BH1750_POWER_ON);
    sleep_ms(10); // Pequeno delay para estabilização após ligar.
    bh1750_set_mode(i2c_instance, BH1750_CONT_HIGH_RES_MODE);
    
    // 5. Aguarda o tempo da primeira conversão (máx. 180ms para alta resolução).
    sleep_ms(180);

    printf("BH1750 configurado e pronto para leitura.\n");

    // Retorna o ponteiro para a instância I2C para ser usado nas outras funções.
    return i2c_instance;
}

/**
 * @brief Envia um comando de configuração de modo para o sensor BH1750.
 * @param i2c A instância I2C na qual o sensor está conectado.
 * @param mode O byte de comando para o modo de operação desejado.
 */
void bh1750_set_mode(i2c_inst_t *i2c, uint8_t mode) {
    // Escreve um byte de comando no endereço do sensor para configurar seu modo.
    i2c_write_blocking(i2c, BH1750_ADDR, &mode, 1, false);
}

/**
 * @brief Realiza a leitura da intensidade luminosa do sensor.
 * @param i2c A instância I2C na qual o sensor está conectado.
 * @param[out] lux Ponteiro para uma variável float onde o valor de luminosidade (em lux) será armazenado.
 */
void bh1750_read_lux(i2c_inst_t *i2c, float *lux) {
    uint8_t buffer[2];

    // Lê 2 bytes (MSB e LSB) do sensor através do barramento I2C.
    i2c_read_blocking(i2c, BH1750_ADDR, buffer, 2, false);
    
    // Combina os dois bytes (MSB << 8 | LSB) em um único valor inteiro de 16 bits.
    uint16_t raw = (buffer[0] << 8) | buffer[1];
    
    // Converte o valor bruto para lux, conforme a fórmula do datasheet.
    // O fator 1.2 é usado para ajustar a contagem bruta para o valor real em lux.
    *lux = raw / 1.2f;
}