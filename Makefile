.PHONY: all clean test build run nix docker-build docker-run docker-exec docker-stop docker-rm
	
OCI_IMAGE := ghcr.io/nooneknowspeter/cpplings:main

build:
	zig build
run:
	zig build run
nix:
	nix --extra-experimental-features "nix-command flakes" develop
docker-build:
	docker buildx build -t ${OCI_IMAGE} .
docker-run:
	docker run -itd -v .:/cpplings --name cpplings ${OCI_IMAGE}
	
