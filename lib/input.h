#ifndef INPUT_H
#define INPUT_H

#define ADC_GRP1_NUM_CHANNELS       2
#define ADC_GRP1_BUF_DEPTH          1

#include "ch.h"
#include "hal.h"
#include "game_constants.h"
#include "game.h"

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 1 sample of 2 channels, SW triggered.
 * Channels:    IN11, IN10.
 */
static const ADCConversionGroup adcgrpcfg1 = {
FALSE, /* circular     : Circular buffer mode  */
                                              ADC_GRP1_NUM_CHANNELS, /* num_channels : Number of channels    */
                                              NULL, /* end_cb       : End Callback          */
                                              NULL, /* error_cb     : Error Callback        */
                                              0, /* CR1      */
                                              ADC_CR2_SWSTART, /* CR2      */
                                              ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3)
                                                  | ADC_SMPR1_SMP_AN10(
                                                      ADC_SAMPLE_3), /* SMPR1    */
                                              0, /* SMPR2    */
                                              0, /* HTR      */
                                              0, /* LTR      */
                                              0, /* SQR1     */
                                              0, /* SQR2     */
                                              ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)
                                                  | ADC_SQR3_SQ2_N(
                                                      ADC_CHANNEL_IN10) /* SQR3     */
};

/*
 * Functions prototypes.
 */
void init_input(void);
void update_pad_speed(game_t*);

#endif
