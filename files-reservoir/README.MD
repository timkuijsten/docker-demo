# ARPA2 Reservoir disk format

> *This is a small demonstration of a disk store for ARPA2 Reservoir.
> This is paired with an LDAP repository holding meta-data.*

This directory structure is a mixture of example data and additionals to
smoothe the demo.  Documents like this one and Python scripts are not
part of the usual layout of a Reservoir on disk.

The main structure is embellished with symbolic links, which help to
navigate the structures.  The symbolic links are superfluous because the
same data is in LDAP, but we added them for demonstration purposes.


## Use with Docker

We built a [demo repository](https://hub.docker.com/repository/docker/arpa2/files-reservoir) that you can install with

```
docker pull arpa2/files-reservoir
```

If you intend to use the files in your own Docker, you can formulate as
follows in your Dockerfile:

```
# ARPA2 Reservoir demonstration files
FROM arpa2/files-reservoir AS reservoir

# Underlying layers for this image
FROM ...

# Copy files-reservoir into /var/arpa2/reservoir
COPY --from=reservoir /var/arpa2/reservoir /var/arpa2/reservoir
```


## On-Disk Structures

The following levels occur downward from the root of the Reservoir Demo.


### Level 1.  Domain names

Directly under the root of the Reservoir Demo is a list of domain names,
entered in lowercase and without trailing dot.  International names are
represented in UTF-8.

Domains are represented as directories with level 2 contents.


### Level 2a.  Users and Groups

Inasfar as users are explicitly defined under a domain, they are
named `uid=xxx` for user name `xxx`.  User names with international
characters are represented in UTF-8.  Note that the form is not a
valid UUID.

All user-ish names are represented in the same format, including
aliases and pseudonyms, groups and roles.

Users are represented as directories with an index in its contents,
the same format as Level 2c specified below.


### Level 2b.  Collections

Collections are identified by a random UUID, written in lowercase
hexadecimal form with the usual dashes but no braces around them.

Collections are represented as directories with level 3 contents.


### Level 2c.  Index

Indexes are basically stored in LDAP; they are just an education
extra here.  TODO: For minimal expectations of the object store.

Index entries form a string, and reference a Collection UUID.
In the case of the absent string the special name `...` is used.
Note that this absent string is normally hidden from `ls` display.

Indexes are represented as a symbolic link per entry.  The link
is named as the string is, in UTF-8 notation, and references the
proper Collection by its most-direct relative path.


### Level 3a.  Resources

Resources are binary objects, to be treated as a raw byte sequence
that may be any size, including zero, and contain any bytes, including
zero.  It is up to an application to interpret the contents and
assign meaning to them.  Resources are only added and removed as a
whole.

Resources are identified by a random UUID, written in lowercase
hexadecimal form, with the usual dashes but without braces.

Resources are represented on disk as a file holding the raw
binary contents, named by their UUID identity.


### Level 3b.  Resource Names

Since symbolic links are easily distinguished from regular files,
it is possible to add names for resources, and point them to the
various resources.  These are the names as they are defined in
the Collection.  Names are UTF-8 strings.


## Navigation

How to jump around this Reservoir Demo?


### Given a WebDAV URL

To navigate from the root of the Reservoir Demo for a WebDAV URL:

 1. Translate the Reservoir host name into the domain it serves
 2. Enter the domain directory
 3. Only if a user is involved, enter its `uid=xxx` directory
 4. For each Collection (or "directory") on the path, follow its symlink
 5. Determine if the ACL for the last Collection welcomes the client
 6. For the Resource (or "file") on the end of the path, follow its symlink

For example, `https://reservoir.arpa2.org/Negrin/Whispering/12+%2d+Negrin+%2d+Overture+in+D+Minor%2emp3` navigates with:

 1. The domain for `reservoir.arpa2.org` is `arpa2.org`
 2. Enter directory `arpa2.org`
 3. There is no user
 4. Enter directory `Negrin/Whispering`
 5. The symbolic link resolves to `9673fd11-6d10-48a1-905b-8f8599a0584f`; use that string as a Resource Instance with Reservoir's [fixed](http://uuid.arpa2.org) Resource Class `904dfdb5-6b34-3818-b580-b9a0b4f7e7a9` to evaluate access for the requesting client.
 6. If access is granted, respond with `12 - Negrin - Overture in D Minor.mp3`


