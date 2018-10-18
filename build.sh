#!/bin/bash

builddir () {
	docker build -t $1 $1 && docker tag $1 arpa2:$1
}

builddir base

builddir build-bin
builddir build-pip
builddir build-tls

builddir build-quickder-lillydap
builddir build-steamworks
builddir build-tlspool

builddir demo-identityhub
builddir demo-reservoir

