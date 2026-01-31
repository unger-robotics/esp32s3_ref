/**
 * @file    app.hpp
 * @brief   Hauptapplikationsklasse.
 *
 * Kapselt die Applikationslogik. Hardware-Zugriff nur ueber HAL.
 */

#pragma once

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "common/error_codes.h"
#include "hw/gpio_hal.h"

/**
 * @brief Hauptapplikation (Singleton-artig in main.cpp instanziiert).
 *
 * Verwaltet LED-Blink-Logik. Erweiterbar fuer weitere Peripherie.
 */
class App final {
  public:
    App() = default;

    /**
     * @brief Initialisiert Hardware und internen Zustand.
     * @return APP_ERR_SUCCESS bei Erfolg, negativer Fehlercode bei Fehler.
     */
    error_code_e init();

    /**
     * @brief Blockierende Hauptschleife (kehrt nie zurueck).
     */
    void run_forever();

  private:
    gpio_hal_cfg_t _led_cfg{};     /**< LED-Konfiguration */
    bool           _led_on{false}; /**< Aktueller LED-Zustand */

    /**
     * @brief Toggelt den LED-Zustand.
     */
    void _toggle_led();
};
