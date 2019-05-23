# Raspberry Pi build environment for ARPA2

**This is very much WORK IN PROGRESS.**

*This image will take quite a few GB of disk space*, because it loads an entire Raspbian image, and messes around with it: download 2 GB, move to Docker, unzip to ~4 GB, and so on.

*Docker is evil when your disk fills up.*  It will refuse to start again, **and** it will not allow you to cleanup becaues it is not running.  **Have plenty of free disk space!**


# In the Docker, run:
 
```
qemu-system-arm -kernel /root/kernel-qemu-4.4.34-jessie.img -cpu arm1176 -m 256 -M versatilepb -serial stdio -display none -append "root=/dev/sda2 rootfstype=ext4 rw" -hda /root/raspbian.img -redir tcp:5022::22 -no-reboot
```

This will start, rattle about audio setup trouble, and emit its swan song:
```
Uncompressing Linux... done, booting the kernel.
```

After this, it is stuck.  Forever and ever, amen.
