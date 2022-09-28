#ifndef VITAPAD_UAPI_H
#define VITAPAD_UAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#define VITAPAD_ERROR_DRIVER_NOT_REGISTERED		0x91337000
#define VITAPAD_ERROR_DRIVER_NOT_ACTIVATED		0x91337001
#define VITAPAD_ERROR_DRIVER_ALREADY_ACTIVATED	0x91337002


int vitaPadUsbAttached();
void vitaPadStart(void);
void vitaPadStop(void);
void vitaPadUpdateL2Pressed(int pressed);
void vitaPadUpdateR2Pressed(int pressed);
void vitaPadUpdateL3Pressed(int pressed);
void vitaPadUpdateR3Pressed(int pressed);

#ifdef __cplusplus
}
#endif


#endif
