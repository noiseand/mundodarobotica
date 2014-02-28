#include "bootloader.h"
#case
#include

void beep();

void beep() {
set_pwm1_duty(50);
output_high(PIEZO);
delay_ms(50);
set_pwm1_duty(0);
output_low(PIEZO);
}

void main() {

beep();

while (1) {
}
}
