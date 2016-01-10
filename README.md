# SkiHeater
Firmware for low cost bluetooth module HC-04 based on CSR SoC BC417.

To work with this project (and any other projects for that chip) you need to install the BlueLab software which you can get on https://www.csrsupport.com/ after registration as developer. Also there is much docs for CSR products.

To program and debug chip you need programmer, there is two way:
1) LPT programmer (as I now doesn't work on Win7 x64 and later), you must have full support LPT on your computer, becouse USB<->LPT converters doesn't work at all. You can see scheme here http://microsin.ru/images/stories/hard/CSR-LPT-flasher.PNG (you need external power supply for module).
2) USB<->SPI programmer. It can be genuine CSR programmer for 300$ or you can see this project of reverse engeneered project for TI stelleris launchpad (will cost you about 15$ if you haven't it) https://github.com/Frans-Willem/CsrUsbSpiDeviceRE

So, at this moment you must have installed BlueLab and programmer, so you can develop yourself firmware, or use this or another.
Before you will start, get the firmware dump of your module. To do that you can use BlueFlash tool in BlueLab softawre. 

For understanding architecture of crs you may see examples and turtorials in BlueLab\App folder. Also you can try this project https://github.com/pfalcon/blutunode

You must not erase chip before build and debug your own firmware, becouse bluetooth stack doesn't providing and to save it, you need to build VM project, that project wouldn't rewrite bluetooth stack and will work under it. 


