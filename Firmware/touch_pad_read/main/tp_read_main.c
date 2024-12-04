#include "driver_init.h"


void app_main() {
	
	touch_init();

    while (true) {
		read_buttons();
    }
}
