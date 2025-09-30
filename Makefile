.PHONY: all clean test build run nix docker-build docker-run docker-exec docker-stop docker-rm benchmark
	
OCI_IMAGE := ghcr.io/nooneknowspeter/cpplings:main

run:
	zig build cli
compile-commands:
	zig build compile-flags
nix-shell:
	nix --extra-experimental-features "nix-command flakes" develop
docker-build:
	docker buildx build -t ${OCI_IMAGE} .
docker-run:
	docker run -itd -v .:/cpplings --name cpplings ${OCI_IMAGE}
docker-exec:
	docker exec -it -w "/cpplings" cpplings bash
docker-stop:
	docker container stop cpplings
docker-rm:
	docker rm -f cpplings
format:
	treefmt
lint:
	treefmt --ci --config-file treefmt.lint.toml
benchmark:
	hyperfine -i "zig build cli"

