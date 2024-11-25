#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[3];

void AD_Init(void);

float AD_GetMq2(void);

float AD_GetLx(void);

//float AD_GetLuxData(void);

#endif
