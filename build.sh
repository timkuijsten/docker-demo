#!/bin/bash


# export DOCKER=sudo docker


# The following bash macro accepts numerous forms, and even defaults to
# the current directory.  It will use the build directory name to tag the
# image, and also tag it under arpa2.
#
# builddir <subdirname>
# builddir <absdirname>
# builddir .
# builddir
#
builddir () {
	DIR=$( cd ${1:-.} ; pwd )
	TAG=$(basename ${DIR:-.})
	${DOCKER:-docker} build -t "$TAG" "$DIR" && ${DOCKER:-docker} tag "$TAG" arpa2:"$TAG" && touch "$DIR"/.built
}

builddir base

builddir build-bin
builddir build-pip
builddir build-libtls

builddir build-quickder-lillydap
builddir build-steamworks
builddir build-tlspool
#HEAVY_IMAGE_2.3GB# builddir build-qpid
#HEAVY_DEMO__2.3GB# builddir wip-messaging
#HEAVY_IMAGE_1.3GB# builddir build-apachemod

builddir demo-identityhub
builddir demo-reservoir

# Reconstruct the DOT image

cd $(basename "$0")
./deps.sh > deps.dot
dot -T png deps.dot > deps.png

