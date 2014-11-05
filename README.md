simple_one
---------

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

