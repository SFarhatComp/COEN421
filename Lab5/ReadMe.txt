This Lab makes use of multiple classes in order to compute the TCP CONNECTION needed to link the Raspberry Pi to the ROBOT. 

THE TIMER CLASS is implemented in order to call time and wait. QNX does not offer the os time per default. Wwe need a timer

The CSocket class is needed to create the TCP connection . It creates an object that links to the Robot.

The Nedclient is the class that takes care of the commands. The commands are formatted in a json format, then sent to the robot in order to perform manipulations.

Finally the main class is just used for test. We call the objects for the proper class, and call them 
