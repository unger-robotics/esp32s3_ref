/**
 * @file    app_config.h
 * @brief   Zentrale Konfigurationskonstanten (Datenstand).
 *
 * Alle Magic Numbers gehoeren hierher, nicht in den Code.
 * Board-spezifische Pins im Datenblatt pruefen.
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "driver/gpio.h"

/* ============================================================================
 * KONFIGURATION - HARDWARE
 * ============================================================================ */

/** @brief GPIO-Pin fuer Status-LED (Board-abhaengig). */
#define APP_BLINK_GPIO (GPIO_NUM_21)

/** @brief Blinkperiode in Millisekunden. */
#define APP_BLINK_PERIOD_MS (500U)

/* ============================================================================
 * KONFIGURATION - SERIAL
 * ============================================================================ */

/** @brief Baudrate fuer Serial-Ausgabe. */
#define APP_SERIAL_BAUD (115200U)

/** @brief Wartezeit nach Serial.begin() damit Monitor verbinden kann (ms). */
#define APP_SERIAL_STARTUP_MS (2000U)

/** @brief Poll-Intervall beim Warten auf Serial (ms). */
#define APP_SERIAL_POLL_MS (10U)

/** @brief Delay bei kritischem Fehler in Millisekunden. */
#define APP_ERROR_DELAY_MS (1000U)

/* ============================================================================
 * KONFIGURATION - VERSION
 * ============================================================================ */

/** @brief Applikationsname. */
#define APP_NAME "esp32s3_ref"

/** @brief Versionsnummern. */
#define APP_VERSION_MAJOR (1U)
#define APP_VERSION_MINOR (0U)
#define APP_VERSION_PATCH (0U)

#endif /* APP_CONFIG_H_ */
