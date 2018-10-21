#!/bin/bash

builddir () {
	DIR="${1%/}"
	docker build -t "$DIR" "$DIR" && docker tag "$DIR" arpa2:"$DIR" && touch "$DIR"/.built
}

builddir base

builddir build-bin
builddir build-pip
builddir build-tls

builddir build-quickder-lillydap
builddir build-steamworks
builddir build-tlspool
builddir build-tls

builddir demo-identityhub
builddir demo-reservoir

