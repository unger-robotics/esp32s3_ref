/**
 * @file    serial_hal.cpp
 * @brief   Implementierung der Serial Hardware-Abstraktionsschicht.
 *
 * Als C++ kompiliert wegen Arduino Serial-Objekt,
 * aber mit extern "C" Linkage fuer C-Kompatibilitaet.
 */

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "hw/serial_hal.h"

#include "common/app_config.h"

#include <Arduino.h>
#include <cstdio>

/* ============================================================================
 * PRIVATE KONSTANTEN
 * ============================================================================ */

/** @brief Maximale Laenge einer Log-Zeile. */
static constexpr size_t LOG_BUFFER_SIZE = 128U;

/** @brief Trennlinie fuer Banner. */
static const char SEPARATOR[] = "----------------------------------------";

/** @brief Level-Strings fuer Log-Ausgabe. */
static const char *const LEVEL_STRINGS[] = {"DEBUG", "INFO ", "WARN ", "ERROR"};

/* ============================================================================
 * OEFFENTLICHE FUNKTIONEN (extern "C")
 * ============================================================================ */

extern "C" {

error_code_e serial_hal_init(uint32_t baud, uint32_t startup_ms)
{
    Serial.begin(baud);

    /* Warten bis Serial bereit (USB CDC) */
    while (!Serial) {
        delay(APP_SERIAL_POLL_MS);
    }

    /* Zusaetzliche Wartezeit damit Monitor verbinden kann */
    if (startup_ms > 0U) {
        delay(startup_ms);
    }

    /* Bildschirm leeren und Cursor nach oben (ANSI Escape) */
    Serial.print("\033[2J\033[H");

    return APP_ERR_SUCCESS;
}

void serial_hal_printf(const char *format, ...)
{
    if (format == nullptr) {
        return;
    }

    char buffer[LOG_BUFFER_SIZE];
    va_list args;

    va_start(args, format);
    (void)vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.print(buffer);
}

void serial_hal_log(log_level_e level, const char *tag, const char *format, ...)
{
    if ((format == nullptr) || (tag == nullptr)) {
        return;
    }

    /* Level validieren */
    if (level > LOG_LEVEL_ERROR) {
        level = LOG_LEVEL_ERROR;
    }

    char buffer[LOG_BUFFER_SIZE];
    va_list args;

    va_start(args, format);
    (void)vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    /* Format: [LEVEL] [tag] message */
    Serial.printf("[%s] [%s] %s\n", LEVEL_STRINGS[level], tag, buffer);
}

void serial_hal_separator(void)
{
    Serial.println(SEPARATOR);
}

} /* extern "C" */
