#ifndef __PIN_PAD_H__
#define __PIN_PAD_H__

int failed;
int passed;
int puk_enabled;
int timeOut;

void configGraphic(void *arg0);

void configKeyPad(void *arg0);
void checkPinPad(void *arg0);

#endif

