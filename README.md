kernel_keylogger
---------

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

And unregister/remove

```
sudo rmmod simple_one
```

