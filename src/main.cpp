/**
 * @file    main.cpp
 * @brief   Arduino Entry-Point (setup/loop).
 *
 * Initialisiert Serial mit Startup-Delay, zeigt Banner,
 * und uebergibt Kontrolle an die Applikation.
 */

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "app/app.hpp"

#include "common/app_config.h"
#include "hw/serial_hal.h"

#include <Arduino.h>

/* ============================================================================
 * PRIVATE KONSTANTEN
 * ============================================================================ */

/** @brief Modul-Tag fuer Log-Ausgaben. */
static const char *const TAG = "Main";

/* ============================================================================
 * GLOBALE VARIABLEN
 * ============================================================================ */

/** @brief Applikationsinstanz (statisch, Lebensdauer = Programmlaufzeit). */
static App g_app;

/* ============================================================================
 * PRIVATE FUNKTIONEN
 * ============================================================================ */

/**
 * @brief Gibt Startup-Banner mit Versionsinformationen aus.
 */
static void printBanner(void)
{
    serial_hal_separator();
    serial_hal_printf("  %s v%u.%u.%u\n",
                      APP_NAME,
                      APP_VERSION_MAJOR,
                      APP_VERSION_MINOR,
                      APP_VERSION_PATCH);
    serial_hal_printf("  Board: Seeed XIAO ESP32S3\n");
    serial_hal_separator();
    serial_hal_printf("\n");
}

/* ============================================================================
 * ARDUINO ENTRY POINTS
 * ============================================================================ */

/**
 * @brief Arduino Setup-Funktion (einmalig nach Reset).
 */
void setup()
{
    /* Serial mit Startup-Delay initialisieren */
    (void)serial_hal_init(APP_SERIAL_BAUD, APP_SERIAL_STARTUP_MS);

    /* Startup-Banner ausgeben */
    printBanner();

    serial_hal_log(LOG_LEVEL_INFO, TAG, "Initialisiere...");

    /* Applikation initialisieren */
    const error_code_e st = g_app.init();
    if (st < 0) {
        serial_hal_log(LOG_LEVEL_ERROR, TAG, "Init fehlgeschlagen: %d",
                       static_cast<int>(st));

        /* Fehler: Endlosschleife */
        for (;;) {
            delay(APP_ERROR_DELAY_MS);
        }
    }

    serial_hal_log(LOG_LEVEL_INFO, TAG, "Applikation gestartet");
    serial_hal_printf("\n");

    /* Kontrolle an Hauptschleife uebergeben (blockiert) */
    g_app.run_forever();
}

/**
 * @brief Arduino Loop-Funktion (ungenutzt, da run_forever() blockiert).
 */
void loop()
{
    /* Nicht erreichbar */
}
