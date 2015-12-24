#include <Time.h>
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

void processSyncMessage(String msg) {
  // if time sync available from serial port, update time and return true
  int i;
  time_t pctime = 0;
  for (i = 1; i < msg.length(); i++) {
    if (isdigit(msg[i])) {
      pctime = (10L * pctime) + (msg[i] - '0');
    }
  }
  pctime -= 8L * 3600L;
  setTime(pctime);
}
