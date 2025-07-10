
#include <stdio.h>
#include "pico/stdlib.h"
#include "init.h"
#include "bh1750.h"
#include "servo.h"

/**
 * @brief Mapeia um valor de uma faixa de entrada para uma faixa de saída.
 * Especificamente, converte a leitura de lux para um ângulo de servo (0-180).
 * @param lux O valor de luminosidade lido do sensor.
 * @param lux_min O valor mínimo de lux para iniciar o movimento (corresponde a 0 graus).
 * @param lux_max O valor máximo de lux para o movimento completo (corresponde a 180 graus).
 * @return O ângulo correspondente para o servo, um valor entre 0 e 180.
 */
uint map_lux_to_angle(float lux, float lux_min, float lux_max) {
    // Garante que o valor de lux não saia da faixa de controle.
    if (lux < lux_min) {
        return 0;
    }
    if (lux > lux_max) {
        return 180;
    }
    // Aplica uma fórmula de mapeamento linear (regra de três).
    long angle = (lux - lux_min) * 180 / (lux_max - lux_min);
    return (uint)angle;
}

/**
 * @brief Função principal do programa.
 * @return int Código de saída do programa (nunca deve retornar).
 */
int main() {
    // Inicializa todos os sistemas: stdio, I2C do display, e o display.
    initializeSystem();
    
    // Inicializa o servo motor.
    servo_init();
    sleep_ms(1000); // Delay para o servo se estabilizar na posição inicial.

    printf("Iniciando leitura de luminosidade e controle do servo...\n");
    // Inicializa o sensor BH1750 no barramento i2c0, pinos 0 e 1.
    i2c_inst_t *i2c_bh1750 = bh1750_init(i2c0, 0, 1);

    //  Parâmetros de calibração do sistema 
    const float LUX_MIN_PARA_SERVO = 50.0;  
    const float LUX_MAX_PARA_SERVO = 1000.0; 

    while (1) {
        //  Leitura do Sensor
        float lux_value;
        bh1750_read_lux(i2c_bh1750, &lux_value);

        // Processamento 
        // Mapeia o valor de lux para um ângulo de 0 a 180.
        uint servo_angle = map_lux_to_angle(lux_value, LUX_MIN_PARA_SERVO, LUX_MAX_PARA_SERVO);
        
        // Atuação e Feedback
        // Comanda o servo para a nova posição.
        servo_set_angle(servo_angle);

        // Imprime os valores no monitor serial para depuração.
        printf("Luminosidade: %.2f lux, Angulo do Servo: %u graus\n", lux_value, servo_angle);
        
        // Prepara a string e a exibe no display OLED.
        char lux_str[32];
        sprintf(lux_str, "Luz: %.1f lux", lux_value);
        updateTextLine(lux_str, 5, 10, 2, 128); 

        sprintf(lux_str, "Servo: %u graus", servo_angle);
        updateTextLine(lux_str, 5, 35, 2, 128); 
        
        // Envia o conteúdo do buffer para a tela do display.
        ssd1306_show(&display);
        
        // Aguarda um tempo antes da próxima iteração do loop.
        sleep_ms(200);
    }
    return 0;
}