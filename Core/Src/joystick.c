#include "joystick.h"

volatile uint32_t result_prev[adc_channel_count] = { 0 };

StickStateEvent onSticksChanged(uint32_t adc_dma_result[adc_channel_count]) {
	int16_t x = -(adc_dma_result[0] * 255 / 4096 - 127);
	int16_t y = adc_dma_result[1] * 255 / 4096 - 127;
	int16_t Rx = -(adc_dma_result[2] * 255 / 4096 - 127);
	int16_t Ry = adc_dma_result[3] * 255 / 4096 - 127;

	StickStateEvent ev;
	ev.Rx = Rx;
	ev.x = x;
	ev.Ry = Ry;
	ev.y = y;
	ev.isChanged = 0;
	if (result_prev[0] != x || result_prev[1] != y) {
		result_prev[0] = x;
		result_prev[1] = y;
		ev.isChanged = 1;
	}
	if (result_prev[2] != Rx || result_prev[3] != Ry) {
		result_prev[2] = Rx;
		result_prev[3] = Ry;
		ev.isChanged = 1;
	}

	return ev;

}
