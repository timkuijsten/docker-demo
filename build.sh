#!/bin/bash

builddir () {
	docker build -t $1 $1 && docker tag $1 arpa2:$1
}

builddir base

builddir build-bin
builddir build-pip

builddir demo-identityhub

