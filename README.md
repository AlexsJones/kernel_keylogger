simple_one
---------

This is a proof of concept
I've not finished the project at time of writing, but the intention is to intercept keycodes, transpose to ASCII and pipe out of the module, preferably with interprocess comms to a userland program that escorts it out into the network
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

