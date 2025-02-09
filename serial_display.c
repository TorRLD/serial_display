/*
 * Exemplo de solução integrando:
 *  - Interface Serial: exibe cada caractere digitado no display OLED (SSD1306).
 *    Se for um dígito (0-9), exibe o símbolo correspondente na matriz 5x5 WS2812.
 *  - Botão A (GPIO5): alterna o LED RGB VERDE (ligado/desligado) e registra a operação
 *    no display OLED e no Serial Monitor.
 *  - Botão B (GPIO6): alterna o LED RGB AZUL (ligado/desligado) e registra a operação
 *    no display OLED e no Serial Monitor.
 *
 * Autor: Heitor Rodrigues Lemos Dias
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "ws2812.pio.h"    // Biblioteca para os WS2812 (certifique-se de que está incluída)
#include "include/ssd1306.h"   // Biblioteca para o display OLED SSD1306
#include "include/font.h"      // Fonte para o display

// ================================
// DEFINIÇÕES DE CONSTANTES E PINOS
// ================================

// Botões
#define BOTAO_A_PIN           5    // Botão A: alterna LED VERDE
#define BOTAO_B_PIN           6    // Botão B: alterna LED AZUL

// LED RGB: usamos dois canais para o exemplo
#define LED_RGB_VERDE_PIN     11   // Canal verde
#define LED_RGB_AZUL_PIN      12   // Canal azul

// WS2812 – matriz 5x5 de 25 LEDs
#define WS2812_PIN            7
#define NUM_PIXELS            25

// Parâmetros para debounce
#define ATRASO_DEBOUNCE_US    200000   // 200 ms

// Cores para os WS2812 (quando um dígito é exibido)
#define COR_WS2812_R          0
#define COR_WS2812_G          0
#define COR_WS2812_B          100

// Configuração do display OLED via I2C
#define I2C_PORT              i2c1
#define I2C_SDA               14
#define I2C_SCL               15
#define SSD1306_ADDRESS       0x3C
#define SSD1306_WIDTH         128
#define SSD1306_HEIGHT        64

// ================================
// VARIÁVEIS GLOBAIS
// ================================

// Buffer para a matriz WS2812: cada posição indica se o LED deve acender (true) ou não (false).
bool buffer_leds[NUM_PIXELS] = { false };

// Padrões dos dígitos (matriz 5x5 – estilo digital)
const bool padroes_digitos[10][5][5] = {
    // Dígito 0
    { {true,  true,  true,  true,  true},
      {true,  false, false, false, true},
      {true,  false, false, false, true},
      {true,  false, false, false, true},
      {true,  true,  true,  true,  true} },
    // Dígito 1
    { {false, false, true,  false, false},
      {false, true,  true,  false, false},
      {true,  false, true,  false, false},
      {false, false, true,  false, false},
      {false, false, true,  false, false} },
    // Dígito 2
    { {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {true,  true,  true,  true,  true},
      {true,  false, false, false, false},
      {true,  true,  true,  true,  true} },
    // Dígito 3
    { {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {true,  true,  true,  true,  true} },
    // Dígito 4
    { {true,  false, false, false, true},
      {true,  false, false, false, true},
      {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {false, false, false, false, true} },
    // Dígito 5
    { {true,  true,  true,  true,  true},
      {true,  false, false, false, false},
      {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {true,  true,  true,  true,  true} },
    // Dígito 6
    { {true,  true,  true,  true,  true},
      {true,  false, false, false, false},
      {true,  true,  true,  true,  true},
      {true,  false, false, false, true},
      {true,  true,  true,  true,  true} },
    // Dígito 7
    { {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {false, false, false, true,  false},
      {false, false, true,  false, false},
      {false, false, true,  false, false} },
    // Dígito 8
    { {true,  true,  true,  true,  true},
      {true,  false, false, false, true},
      {true,  true,  true,  true,  true},
      {true,  false, false, false, true},
      {true,  true,  true,  true,  true} },
    // Dígito 9
    { {true,  true,  true,  true,  true},
      {true,  false, false, false, true},
      {true,  true,  true,  true,  true},
      {false, false, false, false, true},
      {true,  true,  true,  true,  true} }
};

// Variáveis para controle dos botões e dos LEDs (para uso nas interrupções e no loop principal)
volatile absolute_time_t ultimo_debounce_A, ultimo_debounce_B;
volatile bool flag_botaoA = false;
volatile bool flag_botaoB = false;
volatile bool led_verde_state = false;  // LED verde inicialmente desligado
volatile bool led_azul_state = false;   // LED azul inicialmente desligado

// Variáveis para armazenar a última letra digitada e a última mensagem dos botões
char last_serial_char = ' ';
char last_led_message[32] = "";

// ================================
// FUNÇÕES AUXILIARES – WS2812
// ================================

/**
 * Envia um pixel (cor em formato GRB) para os WS2812 via PIO.
 * (O valor é deslocado << 8 conforme o esperado pelo programa PIO.)
 */
static inline void enviar_pixel(uint32_t pixel_grb) {
    // Usa o PIO0, state machine 0 (conforme a inicialização feita em main)
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

/**
 * Converte componentes R, G e B em um valor de 32 bits (formato GRB).
 */
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 8) | ((uint32_t)g << 16) | b;
}

/**
 * Envia a cor para cada LED da matriz WS2812 de acordo com o buffer.
 * Se buffer_leds[i] é true, envia a cor; caso contrário, envia 0 (LED apagado).
 */
void definir_leds(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t cor = urgb_u32(r, g, b);
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (buffer_leds[i])
            enviar_pixel(cor);
        else
            enviar_pixel(0);
    }
    sleep_us(60);  // Aguarda para "travar" os dados no WS2812
}

/**
 * Atualiza o buffer (buffer_leds) com o padrão do dígito desejado (0 a 9).
 */
void atualizar_buffer_com_digito(int digito) {
    for (int linha = 0; linha < 5; linha++) {
        int linha_fisica = 4 - linha;
        for (int coluna = 0; coluna < 5; coluna++) {
            int indice;
            if (linha_fisica == 0 || linha_fisica == 2) {
                indice = linha_fisica * 5 + (4 - coluna);
            } else {
                indice = linha_fisica * 5 + coluna;
            }
            buffer_leds[indice] = padroes_digitos[digito][linha][coluna];
        }
    }
}

// ================================
// ROTINA DE INTERRUPÇÃO PARA OS BOTÕES
// ================================

/**
 * Callback para interrupção nos botões A e B.
 * Usa debounce e, se a condição for satisfeita, alterna o estado do LED correspondente
 * (verde para o botão A e azul para o botão B) e sinaliza que a operação ocorreu.
 */
void callback_gpio(uint gpio, uint32_t eventos) {
    absolute_time_t agora = get_absolute_time();
    if (gpio == BOTAO_A_PIN) {
        if (absolute_time_diff_us(ultimo_debounce_A, agora) > ATRASO_DEBOUNCE_US) {
            led_verde_state = !led_verde_state;
            flag_botaoA = true;
            ultimo_debounce_A = agora;
        }
    } else if (gpio == BOTAO_B_PIN) {
        if (absolute_time_diff_us(ultimo_debounce_B, agora) > ATRASO_DEBOUNCE_US) {
            led_azul_state = !led_azul_state;
            flag_botaoB = true;
            ultimo_debounce_B = agora;
        }
    }
}

// ================================
// FUNÇÃO MAIN
// ================================
int main() {
    stdio_init_all();

    // ----- Inicialização do display OLED (SSD1306) via I2C -----
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_t ssd;
    ssd1306_init(&ssd, SSD1306_WIDTH, SSD1306_HEIGHT, false, SSD1306_ADDRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // ----- Inicialização dos WS2812 via PIO -----
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
    // Inicialmente, todos os LEDs estão apagados (buffer_leds com false)
    definir_leds(COR_WS2812_R, COR_WS2812_G, COR_WS2812_B);

    // ----- Inicialização dos LEDs RGB (canais verde e azul) -----
    gpio_init(LED_RGB_VERDE_PIN);
    gpio_set_dir(LED_RGB_VERDE_PIN, GPIO_OUT);
    gpio_put(LED_RGB_VERDE_PIN, led_verde_state);
    gpio_init(LED_RGB_AZUL_PIN);
    gpio_set_dir(LED_RGB_AZUL_PIN, GPIO_OUT);
    gpio_put(LED_RGB_AZUL_PIN, led_azul_state);

    // ----- Inicialização dos Botões (com pull-up interna) -----
    gpio_init(BOTAO_A_PIN);
    gpio_set_dir(BOTAO_A_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_A_PIN);
    
    gpio_init(BOTAO_B_PIN);
    gpio_set_dir(BOTAO_B_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_B_PIN);

    // Configura as interrupções: para o botão A usamos a função callback comum
    gpio_set_irq_enabled_with_callback(BOTAO_A_PIN, GPIO_IRQ_EDGE_FALL, true, callback_gpio);
    gpio_set_irq_enabled(BOTAO_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // ----- Loop principal -----
    while (true) {
        // Verifica se há caractere disponível no Serial Monitor (modo não bloqueante)
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            char c = (char)ch;
            last_serial_char = c;
            printf("Caractere recebido: %c\n", c);
            // Se o caractere for um dígito entre '0' e '9', exibe o símbolo na matriz WS2812
            if (c >= '0' && c <= '9') {
                int digito = c - '0';
                atualizar_buffer_com_digito(digito);
                definir_leds(COR_WS2812_R, COR_WS2812_G, COR_WS2812_B);
            }
        }

        // Se um dos botões foi pressionado, atualiza a mensagem e registra no Serial Monitor
        if (flag_botaoA) {
            flag_botaoA = false;
            if (led_verde_state) {
                strcpy(last_led_message, "LED Verde Ligado");
                printf("Botao A pressionado: LED Verde Ligado\n");
            } else {
                strcpy(last_led_message, "LED Verde Desligado");
                printf("Botao A pressionado: LED Verde Desligado\n");
            }
        }
        if (flag_botaoB) {
            flag_botaoB = false;
            if (led_azul_state) {
                strcpy(last_led_message, "LED Azul Ligado");
                printf("Botao B pressionado: LED Azul Ligado\n");
            } else {
                strcpy(last_led_message, "LED Azul Desligado");
                printf("Botao B pressionado: LED Azul Desligado\n");
            }
        }
        
        // Atualiza os estados físicos dos LEDs RGB
        gpio_put(LED_RGB_VERDE_PIN, led_verde_state);
        gpio_put(LED_RGB_AZUL_PIN, led_azul_state);

        // Atualiza o display OLED: exibe o caractere recebido e, na parte inferior, a mensagem do LED
        ssd1306_fill(&ssd, false);  // Limpa todo o display
        {
            char linha1[32];
            snprintf(linha1, sizeof(linha1), "Char: %c", last_serial_char);
            ssd1306_draw_string(&ssd, linha1, 0, 0);
        }
        ssd1306_draw_string(&ssd, last_led_message, 0, 50);
        ssd1306_send_data(&ssd);

        sleep_ms(50);  // Pequeno delay para reduzir uso de CPU
    }

    return 0;
}
