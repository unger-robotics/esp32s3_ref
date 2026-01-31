/**
 * @file    error_codes.h
 * @brief   Globale Fehlercodes fuer das gesamte Projekt.
 *
 * Alle Funktionen, die fehlschlagen koennen, geben error_code_e zurueck.
 * Konvention: 0 = Erfolg, negative Werte = Fehler (BARR-C).
 */

#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include <stdint.h>

/* ============================================================================
 * TYPES
 * ============================================================================ */

/**
 * @brief Fehlercodes mit APP_ERR_ Praefix (vermeidet Kollision mit lwIP).
 */
typedef enum {
    APP_ERR_SUCCESS = 0,        /**< Kein Fehler, Operation erfolgreich */
    APP_ERR_NULL_POINTER = -1,  /**< Ungueltiger NULL-Zeiger uebergeben */
    APP_ERR_INVALID_ARG = -2,   /**< Ungueltiges Argument */
    APP_ERR_INVALID_STATE = -3, /**< Ungueltige Zustandsmaschine */
    APP_ERR_HW_FAILURE = -4,    /**< Hardware-Fehler (Treiber/Peripherie) */
    APP_ERR_TIMEOUT = -5        /**< Zeitlimit ueberschritten */
} error_code_e;

#endif /* ERROR_CODES_H_ */
