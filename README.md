## MinSupervisorS2

Minimal supervisor firmware for S2. 

This firmware for the STM32U083 is meant to only sequence 
supply power, set RCW and reset the Layerscape core. 
Afterwards, it enters a loop and will check for _power good_ on 
all the rails, and turn off everything on a power failure.

For de implementation of the full supervisor, check the repository `supervisorDemo`. 
