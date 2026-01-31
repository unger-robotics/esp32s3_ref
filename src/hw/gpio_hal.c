/**
 * @file    gpio_hal.c
 * @brief   Implementierung der GPIO Hardware-Abstraktionsschicht.
 */

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "hw/gpio_hal.h"

/* ============================================================================
 * PRIVATE FUNKTIONEN
 * ============================================================================ */

/**
 * @brief Wandelt bool in GPIO-Pegel (0/1) um.
 *
 * Explizite Konvertierung vermeidet implizite Casts (MISRA).
 *
 * @param[in] v  Boolean-Wert.
 * @return 1 bei true, 0 bei false.
 */
static int boolToLevel(bool v)
{
    return (v != false) ? 1 : 0;
}

/* ============================================================================
 * OEFFENTLICHE FUNKTIONEN
 * ============================================================================ */

error_code_e gpio_hal_init_output(const gpio_hal_cfg_t *p_cfg)
{
    /* Nullzeiger-Pruefung (defensive Programmierung) */
    if (p_cfg == NULL) {
        return APP_ERR_NULL_POINTER;
    }

    /* GPIO-Konfiguration aufbauen */
    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << (uint32_t)p_cfg->pin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    /* ESP-IDF Treiber aufrufen */
    const esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        return APP_ERR_HW_FAILURE;
    }

    /* Startzustand: inaktiv (invertierte Logik bei active-low) */
    (void)gpio_set_level(p_cfg->pin, p_cfg->is_active_high ? 0 : 1);

    return APP_ERR_SUCCESS;
}

error_code_e gpio_hal_write(gpio_num_t pin, bool level)
{
    const esp_err_t err = gpio_set_level(pin, boolToLevel(level));
    return (err == ESP_OK) ? APP_ERR_SUCCESS : APP_ERR_HW_FAILURE;
}
