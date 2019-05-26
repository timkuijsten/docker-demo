# Support for Multiple Architectures

> *Ideally, our code and demos run everywhere.  The Docker Demos
> are primarly to get you acquanted with our work, but it is
> still useful if you can try ARPA2 components on Raspberry Pi,
> or at least see how to build them.*

Multiple is more than one, right?  So if we add some support
for building for `armhf`, the processor of Raspbian, we can call
it that?

Multiarch is a [Debian term](https://wiki.debian.org/Multiarch)
and this platform explains
[how to work](https://wiki.debian.org/Multiarch/HOWTO)
with a system that mixes platforms.  One big constraint:
binary programs are either/or.


We currently have images in parallel for `armhf`.  For instance,
`build-bin` builds for the Docker default platform, but the
more evocative name `build-armhf` selects the Raspbian platform
as its compiler target.

Docker surprised us by being able to run `armhf` executables
without a blink of an eye.  The containers do not reveal how
they do it, but under the hood it almost has to be a combination
of QEMU with BINFMT.  Interestingly about this mix is that it
allows programs to run an `armhf` program without changing to
another file system, network configuration *or even the kernel*.
As if the CPU was was temporary replaced with another model.  Just like
[Vortex ATonce](https://amiga.resource.cx/exp/atonce) used to do!


MultiArch on Debian is a perfect combination with this.  We get
to install a compiler that compiles to `armhf` code and links it.
CMake does not really mind, it thinks it uses the local compiler
and is therefore blissfully unaware.  The only situation where
this brings problems is if dependencies are not installed for
the targeted platform.  Even when CMake thinks it found a required
package on account of its include file, it may still have problems
finding its library, or linking it if it finds one.

Python is a bit odd, it must run for the architecture if it is
to generate library extensions for the target platform.  This
results in very slow-running programs, much more so than a
compiler run for instance.  No idea why.


## Practical Results

Have a look at the change going from generic/default platform
`build-bin` and the specific `build-armhf`:

```
 # Install required packages
 RUN \
+    dpkg --add-architecture armhf && \
     apt-get update && \
     apt-get -y upgrade
 
 ENV DEBIAN_FRONTEND=noninteractive
 
-RUN apt-get install -y python2.7 \
-	build-essential \
+RUN apt-get install -y python2.7:armhf \
+	gcc:armhf g++:armhf binutils:armhf make:armhf \
 	git vim nano man tmux \
 	flex bison \
-	rsyslog strace gdb \
-	cmake cmake-curses-gui pkg-config \
-	libldap-dev libsasl2-dev libkrb5-dev \
+	rsyslog strace gdb:armhf \
+	cmake cmake-curses-gui pkg-config:armhf \
+	libldap-dev:armhf libsasl2-dev:armhf libkrb5-dev:armhf \
 	runit
```

And, one level down, the difference between `demo-kip` and
`demo-kip-armhf`:

```
-FROM arpa2/build-bin
+FROM arpa2/build-armhf
 
-RUN apt-get -y install libssl-dev libunbound-dev python-setuptools python-dev
+RUN apt-get -y install libssl-dev:armhf libunbound-dev:armhf python-setuptools:armhf python-dev:armhf
 
 RUN easy_install six pyparsing asn1ate
```

The dependencies are a bit different, but otherwise the trick is to add `:armhf` to build tools.
Those build tools include Python because it might be used to cross-build libraries.  Not sure if
a crossover-version could work; it would surely work faster.  The compiler runs fast enough, but
Python does not like to run on `armhf` via Qemu, and slows down dramatically.


## Is there a Difference at all?

The integration of Multiarch and even implicit Docker support is so easy to use that one
wonders if the binaries are really for the `armhf` platform.  It's like having the
thread-specific activation of an extra 80286 processor of a
[Vortex ATonce](https://amiga.resource.cx/exp/atonceplus)
added to your Amiga 500.


But, `file` does tell the tale of a big difference:

```
amd64# file /usr/local/bin/kipd
/usr/local/bin/kipd: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=1d1dd564762207ece4aff71529be4477bea46a7a, not stripped

armhf# file /usr/local/bin/kipd 
/usr/local/bin/kipd: ELF 32-bit LSB shared object, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 3.2.0, BuildID[sha1]=12ac3a024ca2bfc0023310679949538e60fc0431, not stripped
```

and so does the disassembly of some compiled code,

```
amd64# objdump -D lib/libkip.a | sed '/kipdata_random/,$!d' | head -n 10
00000000000001c9 <kipdata_random>:
     1c9:	55                   	push   %rbp
     1ca:	48 89 e5             	mov    %rsp,%rbp
     1cd:	48 83 ec 40          	sub    $0x40,%rsp
     1d1:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
     1d5:	89 75 d4             	mov    %esi,-0x2c(%rbp)
     1d8:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
     1dc:	8b 45 d4             	mov    -0x2c(%rbp),%eax
     1df:	89 45 e4             	mov    %eax,-0x1c(%rbp)
     1e2:	48 8b 45 c8          	mov    -0x38(%rbp),%rax

armhf#  objdump -D lib/libkip.a | sed '/kipdata_random/,$!d' | head -n 10
00000140 <kipdata_random>:
     140:	b580      	push	{r7, lr}
     142:	b088      	sub	sp, #32
     144:	af00      	add	r7, sp, #0
     146:	60f8      	str	r0, [r7, #12]
     148:	60b9      	str	r1, [r7, #8]
     14a:	607a      	str	r2, [r7, #4]
     14c:	68bb      	ldr	r3, [r7, #8]
     14e:	617b      	str	r3, [r7, #20]
     150:	687b      	ldr	r3, [r7, #4]
```


Maybe KIP should be ported to Z80 too?  Would be rather cool, to pass the data in and out via the ZX Spectrum tape interface.  Lacking any network, the separation of the secrets would be perfect :-D  Closer to home would be a variant based on LLC.
