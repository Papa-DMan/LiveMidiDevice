# LiveMidiDevice
This repo merges my personal MIDI-Controller-DisplayDriver and MIDI-Controller repositories into this public one and also hosts the schematics and pcb designs for the project

The MIDI-Controller uses a tree and node structure. A FRDMK64F Dev board is the root of the tree and can handle up to two nodes right now. It utilizes the I2C bus with an extender chipset from TI to send data and power over an RJ45 8 connector cable down to the nodes that contain up to 8 buttons and 1 expression pedal and 1 led display controlled by and stm32 l412kb dev board.
