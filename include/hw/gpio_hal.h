/**
 * @file    gpio_hal.h
 * @brief   Hardware-Abstraktionsschicht fuer GPIO.
 *
 * Kapselt ESP-IDF GPIO-Treiber. Applikationscode greift nur
 * ueber diese Schnittstelle auf GPIOs zu.
 */

#ifndef GPIO_HAL_H_
#define GPIO_HAL_H_

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "common/error_codes.h"
#include "driver/gpio.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * TYPES
 * ============================================================================ */

/**
 * @brief Konfiguration fuer einen GPIO-Ausgang.
 */
typedef struct {
    gpio_num_t pin;      /**< GPIO-Pinnummer (ESP-IDF Typ) */
    bool is_active_high; /**< true = High-Aktiv, false = Low-Aktiv */
} gpio_hal_cfg_t;

/* ============================================================================
 * OEFFENTLICHE FUNKTIONEN
 * ============================================================================ */

/**
 * @brief Initialisiert einen GPIO als Ausgang.
 *
 * @param[in] p_cfg  Zeiger auf Konfigurationsstruktur.
 * @return APP_ERR_SUCCESS bei Erfolg, negativer Fehlercode bei Fehler.
 */
error_code_e gpio_hal_init_output(const gpio_hal_cfg_t *p_cfg);

/**
 * @brief Setzt den Pegel eines GPIO-Ausgangs.
 *
 * @param[in] pin    GPIO-Pinnummer.
 * @param[in] level  true = aktiver Pegel, false = inaktiver Pegel.
 * @return APP_ERR_SUCCESS bei Erfolg, negativer Fehlercode bei Fehler.
 *
 * @note Nicht ISR-sicher falls Logging hinzugefuegt wird.
 */
error_code_e gpio_hal_write(gpio_num_t pin, bool level);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GPIO_HAL_H_ */
