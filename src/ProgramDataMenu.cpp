#include "ProgramDataMenu.h"
#include "LcdPrint.h"
#include "GTPowerA610.h"

uint8_t ProgramDataMenu::printItem(int index)
{
	int blink = getBlinkIndex();
	switch(index) {
	case 0:	lcdPrint_P(PSTR("Bat:  "));     BLINK_INDEX(p_.printBatteryString()); break;
	case 1:	lcdPrint_P(PSTR("V:  ")); 		BLINK_INDEX(p_.printVoltageString());	break;
	case 2:	lcdPrint_P(PSTR("Ic: ")); 		BLINK_INDEX(p_.printChargeString()); 	break;
	case 3:	lcdPrint_P(PSTR("Imax: ")); 	BLINK_INDEX(p_.printCurrentString()); break;
	case PROGRAM_DATA_MENU_SIZE-1:
			lcdPrint_P(PSTR("     save")); 						break;
	}
	return 0;
}

bool ProgramDataMenu::editItem(int index, uint8_t key)
{
	int dir = -1;
	if(key == BUTTON_INC) dir = 1;
	dir *= keyboard.getSpeedFactor();

	switch(index) {
	case 0: p_.changeBattery(dir); 	break;
	case 1: p_.changeVoltage(dir); 	break;
	case 2: p_.changeCharge(dir); 		break;
	case 3: p_.changeCurrent(dir); 		break;
	default:
		return false;
	}
	return true;
}

void ProgramDataMenu::editIndex(int index)
{
	startBlinkOff(index);
	ProgramData undo(p_);
	uint8_t key;
	do {
		key =  keyboard.getPressedWithSpeed();
		if(key == BUTTON_DEC || key == BUTTON_INC) {
			editItem(index, key);
			startBlinkOn(index);
			display();
		}
	display();
	} while(key != BUTTON_STOP && key != BUTTON_START);

	stopBlink();
	if(key == BUTTON_STOP)
		p_ = undo;
	p_.check();
	display();
}

bool ProgramDataMenu::edit() {
	bool release = true;
	uint8_t key;
	render();
	do {
		key = run();
		if(key == BUTTON_NONE) release = false;

		if(!release && key == BUTTON_START)  {
			//"save" selected
			if(getIndex() == PROGRAM_DATA_MENU_SIZE - 1) return true;
			editIndex(getIndex());
		}
	} while(key != BUTTON_STOP || release);
	return false;
}

