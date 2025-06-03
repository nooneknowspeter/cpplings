build:
	zig build
run:
	zig build run
nix:
	nix-shell
asdf:
	asdf install zig
docker-build:
	docker buildx build -t cpplings .
