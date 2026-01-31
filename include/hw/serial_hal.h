/**
 * @file    serial_hal.h
 * @brief   Hardware-Abstraktionsschicht fuer serielle Ausgabe.
 *
 * Kapselt Arduino Serial. Stellt sicher, dass fruehe Ausgaben
 * nach Reset nicht verloren gehen (Startup-Delay).
 */

#ifndef SERIAL_HAL_H_
#define SERIAL_HAL_H_

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "common/error_codes.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * TYPES
 * ============================================================================ */

/**
 * @brief Log-Level fuer strukturierte Ausgaben.
 */
typedef enum {
    LOG_LEVEL_DEBUG = 0, /**< Debug-Informationen */
    LOG_LEVEL_INFO = 1,  /**< Normale Informationen */
    LOG_LEVEL_WARN = 2,  /**< Warnungen */
    LOG_LEVEL_ERROR = 3  /**< Fehler */
} log_level_e;

/* ============================================================================
 * OEFFENTLICHE FUNKTIONEN
 * ============================================================================ */

/**
 * @brief Initialisiert serielle Schnittstelle mit Startup-Delay.
 *
 * Wartet nach Initialisierung, damit der Monitor verbinden kann
 * und fruehe Ausgaben nicht verloren gehen.
 *
 * @param[in] baud         Baudrate (z.B. 115200).
 * @param[in] startup_ms   Wartezeit nach Init in Millisekunden.
 * @return APP_ERR_SUCCESS bei Erfolg.
 */
error_code_e serial_hal_init(uint32_t baud, uint32_t startup_ms);

/**
 * @brief Gibt formatierten Text aus (printf-Syntax).
 *
 * @param[in] format  Format-String.
 * @param[in] ...     Variable Argumente.
 */
void serial_hal_printf(const char *format, ...);

/**
 * @brief Gibt Log-Nachricht mit Level und Modul-Tag aus.
 *
 * Format: "[LEVEL] [tag] message"
 *
 * @param[in] level   Log-Level.
 * @param[in] tag     Modul-Name (z.B. "App", "GPIO").
 * @param[in] format  Format-String.
 * @param[in] ...     Variable Argumente.
 */
void serial_hal_log(log_level_e level, const char *tag, const char *format, ...);

/**
 * @brief Gibt Trennlinie aus (fuer Startup-Banner).
 */
void serial_hal_separator(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SERIAL_HAL_H_ */
