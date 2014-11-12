kernel_keylogger
---------
[![Build Status](https://travis-ci.org/AlexsJones/kernel_keylogger.svg)](https://travis-ci.org/AlexsJones/kernel_keylogger)

This is a proof of concept
Use at your own discretion

Finished product will include a pipe (IPC) 

W.I.P

```
make
```

Then insert

```
sudo insmod simple_one.ko
```

And then observe the registration

```
dmesg | tail -1
```

To read from the char device you'll need to create the correct file with 

  - Note that I've assumed your device will register against 250, this may not be the case on your system, so check the dmesg output and adjust the value

```
./link_dev
```



And unregister/remove

```
sudo rmmod simple_one
```

View the device 
```
cat /proc/devices
```
