OCI_IMAGE := ghcr.io/nooneknowspeter/cpplings:main

build:
	zig build
run:
	zig build run
nix:
	nix-shell
asdf:
	asdf install zig
docker-build:
	docker buildx build -t ${OCI_IMAGE} .
docker-run:
	docker run -itd -v .:/cpplings --name cpplings ${OCI_IMAGE}
	
