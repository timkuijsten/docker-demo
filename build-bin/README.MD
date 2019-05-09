

## Debug settings

If you need to use `gdb` you may run into an error

```
Error disabling address space randomization: Operation not permitted
```

All you need to do is add a flag when you run the image:

```
docker run --security-opt seccomp=unconfined -ti <image> bash
```


