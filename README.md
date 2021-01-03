# DAQ
A Dual-Channel, 24-Bit, 256 kS/s Real-Time Hi-Speed USB 2.0 Data Acquisition (DAQ) Device

# Details
The recording of analog measurement signals is often done with so-called data acquisition (DAQ) devices. Commercial devices from [National Instruments](http://www.ni.com) are commonly used, but unfortunately also very expensive. The goal of this project was to develop a first prototype of a low-cost DAQ device that can compete with commercial devices, i.e. both the hardware DAQ board as well as software for receiving and analyzing the data is necessary.

The basic structure of a DAQ is very simple. In principle, only an analog-to-digital converter (ADC) and an FPGA or microcontroller are needed in order to send the measurement data in real time to a computer. I chose the [AD7768 from Analog Devices](https://www.analog.com/en/products/ad7768.html), which offers up to 8 synchronous channels (but I only use 2 channels), a sampling rate of 256 kS/s and a resolution of 24 bit. Each sample consists of 3 data bytes and one status byte. So with the two channels used, the net data rate is 2 x 256 kS/s x 4 byte = 2.048 MByte/s = 16.384 MBit/s.

To keep the hardware cheap and the compatibility high, I decided to use a microcontroller (and not an FPGA) and a USB connection. Since the net data rate is higher than 12 MBit/s (USB Full-Speed), a USB Hi-Speed connection (up to 480 MBit/s) is required. However, most microcontrollers (e.g. the widely used STM32 family) allow USB Hi-Speed connections only with an additional external PHY. Therefore, I chose a microcontroller from Microchip's PIC32MZ family, since they already have the Hi-Speed PHY integrated.

Technical summary of the developed platform:
* ADC: [AD7768BSTZ](https://www.analog.com/en/products/ad7768.html)
* Number of channels: 2
* Sample rate: 256 kS/s
* Resolution: 24 bit
* Reference voltage: 4.096 V
* Microcontroller: [PIC32MZ2048EFH064](https://www.microchip.com/wwwproducts/en/PIC32MZ2048EFH064)
* USB 2.0 Hi-Speed (net data rate: 16.384 MBit/s)
* Noise: See below

If you have any questions, feel free to contact me!

# Schematics
Please see subdirectory *schematics*.

# Board
The PCB was designed in Autodesk EAGLE 9.6.2 and manufactured by [JLCPCB](https://jlcpcb.com/).

<p align="center">
  <img src="https://github.com/yildi1337/DAQ/blob/main/board/topview.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQ/blob/main/board/bottomview.png" />
</p>

# Firmware
The software running on the PIC32MZ2048EFH064 has been programmed without making use of Microchip's Harmony (the complete source code cann be found in the subdirectory *firmware*). For the development and debugging I used Microchip's MPLAB X IDE v5.40 and the XC32 Toolchain (v2.41). For programming I used the MPLAB PICkit 4 In-Circuit Debugger.

# USB
Unfortunately, programming the USB interface without using Harmony is not trivial, especially because the manufacturer's documentation is very poor. There is a helpful thread in the [Microchip forum](https://www.microchip.com/forums/m1083508.aspx), but it is very tedious to get the interface working this way. Maybe the source code from this project is helpful for someone. The following screenshot shows the details after the successful USB enumeration.

<p align="center">
  <img src="https://github.com/yildi1337/DAQ/blob/main/pictures/usb_properties.png" />
</p>

# USB Driver and USB Communication
On the computer side, standard USB drivers such as [WinUSB](https://en.wikipedia.org/wiki/WinUSB) or [libusb-win32](https://sourceforge.net/projects/libusb-win32/) can be used. A user-friendly installation is possible e.g. via [Zadig](https://zadig.akeo.ie/). However, since it is much more comportable, I decided to use the NI-VISA driver from National Instruments than can be installed via the [NI-VISA Driver Wizard](https://knowledge.ni.com/KnowledgeArticleDetails?id=kA03q000000x1qzCAA&l=en-US). This results in the great advantage that communication can take place by means of the well-known functions fopen(), fread(), fwrite() and fclose(). Corresponding possibilities for communication with an NI-VISA device are available in various languages such as Python, .NET, C++, Java, Matlab, etc.

# Example Software (Matlab)
An example live demo script that fetches the data via USB from the DAQ hardware and plots the results both in the time-domain and in the frequency-domain can be found in the subdirectory *matlab*. Below is a screenshot and video of the live demo program in action (I generated various sinusoidal signals using a PeakTech 4025 DDS function generator).

<p align="center">
  <img src="https://github.com/yildi1337/DAQ/blob/main/matlab/LiveDemo.png" />
</p>

<p align="center">
    <a href="https://www.youtube.com/watch?v=n9_hw3k9drk" title="Live Demo of Dual-Channel, 24-Bit, 256 kS/s Real-Time Hi-Speed USB 2.0 Data Acquisition (DAQ) Device"><img src="http://img.youtube.com/vi/n9_hw3k9drk/0.jpg" /></a>
</p>

# Noise
The voltage noise densities were measured for various cases (open inputs, shortened inputs, and 50 Ohm resistors connected to the inputs). In addition, all measurements were performed for the DAQ being supplied by the USB bus and external mains.

<p align="center">
  <img src="https://github.com/yildi1337/DAQ/blob/main/matlab/NoiseResults.png" />
</p>
