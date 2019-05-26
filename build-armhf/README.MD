
This is a variation of the `build-bin` module but the programs
used for building target `armhf` platforms (which include Raspbian).


## Debug settings

If you need to use `gdb` you may run into an error

```
Error disabling address space randomization: Operation not permitted
```

All you need to do is add a flag when you run the image:

```
docker run --security-opt seccomp=unconfined -ti <image> bash
```


