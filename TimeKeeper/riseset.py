#!/usr/bin/env python

import datetime
from astral import Astral, Location
import pytz

def dt2min(dt):
    return dt.hour * 60 + dt.minute

loc = Location(("Santa Clara", "California", 37.21, -121.58, "US/Pacific", 22))
loc.solar_depression = 'civil'
oneday = datetime.timedelta(days=1)
soy = datetime.date(2016, 1, 1)
for  i in (range(1, 366)):
    sun = loc.sun(date=soy, local=True)
    print("{{ 0x{0:X}, 0x{1:X} }},".format(dt2min(sun['sunrise']), dt2min(sun['sunset'])))
    soy = soy + oneday
