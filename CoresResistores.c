#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

#define I2C_PORT    i2c1
#define I2C_SDA     14
#define I2C_SCL     15
#define OLED_ADDR   0x3C
#define ADC_PIN     28
#define BUTTON_B    6

const int   R_KNOWN       = 10000;   // Resistor fixo de 10kΩ
const float ADC_REF_V     = 3.31f;   // Tensão de referência do ADC
const int   ADC_RES       = 4095;    // Resolução de 12 bits

const char *color_names[10] = {
    "Preto", "Marrom", "Vermelho", "Laranja", "Amarelo",
    "Verde", "Azul", "Violeta", "Cinza", "Branco"
};

// Lista de resistores comerciais
const float resistores_comerciais[] = {
    1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0,
    3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1
};

// Handler para BOOTSEL via USB
#include "pico/bootrom.h"
void gpio_irq_handler(uint gpio, uint32_t events) {
    reset_usb_boot(0, 0);
}

// Função para encontrar o resistor comercial mais próximo
float encontrar_resistor_comercial_proximo(float valor) {
    float escala = 1.0f;
    while (valor >= 10.0f) {
        valor /= 10.0f;
        escala *= 10.0f;
    }
    while (valor < 1.0f) {
        valor *= 10.0f;
        escala /= 10.0f;
    }

    float menor_diferenca = 1000.0f;
    float valor_proximo = 1.0f;

    for (int i = 0; i < sizeof(resistores_comerciais)/sizeof(float); i++) {
        float diff = fabs(valor - resistores_comerciais[i]);
        if (diff < menor_diferenca) {
            menor_diferenca = diff;
            valor_proximo = resistores_comerciais[i];
        }
    }
    return valor_proximo * escala;
}

int main() {
    stdio_init_all();

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_t oled;
    ssd1306_init(&oled, WIDTH, HEIGHT, false, OLED_ADDR, I2C_PORT);
    ssd1306_config(&oled);
    ssd1306_fill(&oled, false);
    ssd1306_send_data(&oled);

    adc_init();
    adc_gpio_init(ADC_PIN);

    char str_adc[6];
    char str_rx[8];

    while (1) {
        adc_select_input(2);
        float sum = 0.0f;
        for (int i = 0; i < 500; i++) {
            sum += adc_read();
            sleep_ms(1);
        }
        float avg = sum / 500.0f;
        float Rx = (R_KNOWN * avg) / (ADC_RES - avg);

        // Aplica o arredondamento para o valor comercial
        float Rx_arredondado = encontrar_resistor_comercial_proximo(Rx);

        sprintf(str_adc, "%1.0f", avg);
        sprintf(str_rx, "%1.0f", Rx_arredondado);

        // Determina as três bandas de cor
        int exp = 0;
        float tmp = Rx_arredondado;
        while (tmp >= 100.0f) {
            tmp /= 10.0f;
            exp++;
        }
        while (tmp < 10.0f && exp > -2) {
            tmp *= 10.0f;
            exp--;
        }
        int iv = (int)(tmp + 0.5f);
        int d1 = iv / 10;
        int d2 = iv % 10;

        const char *band1 = color_names[d1];
        const char *band2 = color_names[d2];
        char band3[10];
        if (exp >= 0 && exp <= 9) {
            strcpy(band3, color_names[exp]);
        } 

        // Exibe no OLED
        ssd1306_fill(&oled, false);

       // Linha 1: ADC
       ssd1306_draw_string(&oled, "ADC:", 0, 0);
       ssd1306_draw_string(&oled, str_adc, 40, 0);
       
       // Linha 2: Resistência
       ssd1306_draw_string(&oled, "Ohms:", 0, 12);
       ssd1306_draw_string(&oled, str_rx, 40, 12);
       
       // Linha 3, 4 e 5: Cores separadas
       ssd1306_draw_string(&oled, "Cor1:", 0, 24);
       ssd1306_draw_string(&oled, band1, 40, 24);
       
       ssd1306_draw_string(&oled, "Cor2:", 0, 36);
       ssd1306_draw_string(&oled, band2, 40, 36);
       
       ssd1306_draw_string(&oled, "Mult:", 0, 48);
       ssd1306_draw_string(&oled, band3, 40, 48);
       
       ssd1306_send_data(&oled);
       

       sleep_ms(700);
    }

    return 0;
}
