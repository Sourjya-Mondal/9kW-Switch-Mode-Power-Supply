# Hind-Rectifiers-9kW-SMPS
9kW Current-Fed DC-DC Converter

This project consists of a current fed converter.
Consider an H-Bridge converter where the duty cycle of the bottom switches is kept at constant 50% while the duty cycle of the top switches is varied between 50% to 70%.
This turns on both the top and bottom switches of that particular leg and shorts the path when the pwm signal of the same overlaps to high.
This seems counter-intuitive and a big red flag, however this is done inorder to store energy in the inductor in this time period.
And dump this stored energy onto the load in the remaining time period.
This topology was developed for electroplating applications were current fed converters are a requirement.
One of the biggest advantages of this topology is that multiple units can easily be connected in parallel (albeit in master-slave configuration).

