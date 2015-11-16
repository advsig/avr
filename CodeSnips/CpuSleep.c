/* AVR Sleep */

set_sleep_mode(SLEEP_MODE_PWR_DOWN);
cli();
sleep_enable();
sleep_bod_disable();
sei();
sleep_cpu();
/* wake up here */
sleep_disable();