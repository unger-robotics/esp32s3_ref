/**
 * @file    app.cpp
 * @brief   Implementierung der Hauptapplikationsklasse.
 */

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "app/app.hpp"

#include "common/app_config.h"
#include "hw/serial_hal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ============================================================================
 * PRIVATE KONSTANTEN
 * ============================================================================ */

/** @brief Modul-Tag fuer Log-Ausgaben. */
static const char *const TAG = "App";

/* ============================================================================
 * OEFFENTLICHE METHODEN
 * ============================================================================ */

error_code_e App::init()
{
    serial_hal_log(LOG_LEVEL_DEBUG, TAG, "GPIO %d als LED konfigurieren",
                   static_cast<int>(APP_BLINK_GPIO));

    /* LED-Konfiguration aus zentraler Config uebernehmen */
    _led_cfg.pin = APP_BLINK_GPIO;
    _led_cfg.is_active_high = true;

    const error_code_e err = gpio_hal_init_output(&_led_cfg);
    if (err != APP_ERR_SUCCESS) {
        serial_hal_log(LOG_LEVEL_ERROR, TAG, "GPIO Init fehlgeschlagen");
        return err;
    }

    serial_hal_log(LOG_LEVEL_INFO, TAG, "LED initialisiert (GPIO %d)",
                   static_cast<int>(_led_cfg.pin));

    return APP_ERR_SUCCESS;
}

void App::run_forever()
{
    serial_hal_log(LOG_LEVEL_INFO, TAG, "Hauptschleife gestartet (Periode: %u ms)",
                   APP_BLINK_PERIOD_MS);

    /* Endlosschleife - kehrt nie zurueck */
    for (;;) {
        _toggle_led();
        vTaskDelay(pdMS_TO_TICKS(APP_BLINK_PERIOD_MS));
    }
}

/* ============================================================================
 * PRIVATE METHODEN
 * ============================================================================ */

void App::_toggle_led()
{
    _led_on = !_led_on;
    /* Rueckgabewert ignoriert: LED-Fehler nicht kritisch */
    (void)gpio_hal_write(_led_cfg.pin, _led_on);
}
