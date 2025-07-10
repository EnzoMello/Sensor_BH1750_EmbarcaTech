
#ifndef BH1750_I2C_H
#define BH1750_I2C_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições de pinos para o I2C0 da BitDogLab (ou outra placa)
#define I2C0_SDA_PIN 0 ///< Pino GPIO utilizado para a linha de dados (SDA) do I2C0.
#define I2C0_SCL_PIN 1 ///< Pino GPIO utilizado para a linha de clock (SCL) do I2C0.

/**
 * @brief Inicializa o barramento I2C e o sensor BH1750.
 * @param i2c_instance A instância I2C a ser usada (ex: i2c0, i2c1).
 * @param sda_pin O pino GPIO para a linha SDA.
 * @param scl_pin O pino GPIO para a linha SCL.
 * @return Retorna um ponteiro para a instância I2C inicializada para uso futuro.
 */
i2c_inst_t* bh1750_init(i2c_inst_t *i2c_instance, uint sda_pin, uint scl_pin);

/**
 * @brief Envia um comando de configuração de modo para o sensor BH1750.
 * @param i2c A instância I2C na qual o sensor está conectado.
 * @param mode O byte de comando para o modo de operação desejado (ex: ligar, modo de medição).
 */
void bh1750_set_mode(i2c_inst_t *i2c, uint8_t mode);

/**
 * @brief Realiza a leitura da intensidade luminosa do sensor.
 * @param i2c A instância I2C na qual o sensor está conectado.
 * @param[out] lux Ponteiro para uma variável float onde o valor de luminosidade (em lux) será armazenado.
 */
void bh1750_read_lux(i2c_inst_t *i2c, float *lux);

#endif BH1750_I2C_H