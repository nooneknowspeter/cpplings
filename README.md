# cpplings

**cpplings**, inspired by [Rustlings](https://github.com/rust-lang/rustlings), 
is a set of hands-on exercises to help you get comfortable with reading 
and writing modern **C++**.

These exercises reference the excellent [learncpp](https://www.learncpp.com/) 
and are conceptually related to [cplings](https://github.com/rdjondo/cplings), 
which adopted this idea earlier.

We recommend doing these exercises alongside reading [learncpp](https://www.learncpp.com/) 
and checking out the additional learning resources listed below.

---

## Why Zig?

This project uses **[Zig](https://ziglang.org/)** as a toolchain instead of CMake. 
This is a deliberate choice to test Zig’s practical usefulness in real-world C++ workflows.

### Benefits of Using Zig

- Zig is simple but powerful.
- It ships with a Clang-based compiler toolchain built in.
- It simplifies compiling for multiple architectures (x86, ARM, etc.).
- It removes the pain of installing and managing compiler versions manually (GCC, Clang, etc.).
- Zig-based builds are highly reproducible and consistent across platforms.

---

## Build & Setup

This project uses a [`Makefile`](./Makefile) to simplify common development tasks.
You will need `make` installed on your system.


| Command | Description |
| -------------- | --------------- |
| make build | Builds the project using `zig build`, the executable can be found in `./zig-out/bin/cpplings`  |
| make run | Build the project like `zig build` then runs the executable |
| make nix | Runs a `nix-shell` using the `shell.nix` configuration in the project |
| make asdf | Installs the specified version of Zig in the `.tool-versions` |
| make docker-build | Builds a local Docker image |
| make docker-run | Starts a container using the `cpplings` image and mounts a volume to the path of the clone repository |


### Local Setup with asdf (recommended)

We recommend using asdf to manage tool versions in a clean and portable way.
This project includes a `.tool-versions` file to lock the required Zig version.

Install and build the project locally:

```sh
make asdf
make build
make run
```

This ensures a consistent development environment across machines.

### Nix Shell

If you're using Nix, a nix-shell environment is provided with all required dependencies,
such as Zig, asdf, and more.

Start the environment with:

```sh
make nix
```

This gives you a fully configured development shell without installing the dependencies globally on your system.
You just need to have Nix installed.

### Docker

A Dockerfile is provided for containerized development.

You can build and run the container locally using:

```sh
make docker-build
make docker-run
```

The image will be tagged as `cpplings`.

The container mounts your current project directory into `/cpplings` in the container.

This setup is ideal for isolated or reproducible builds.

A pre-built image is also available via GitHub Container Registry by running:

```sh
make docker-run
```

---

## Resources

[learncpp](https://www.learncpp.com/) — Comprehensive and beginner-friendly C++ guide.

[cppreference](https://en.cppreference.com/w/) — The canonical C++ reference.

[cppbyexample](https://cppbyexample.com/) — C++ concepts via concise examples.

[learnxinyminutes: C++](https://learnxinyminutes.com/docs/c++) — Fast overview for experienced programmers.

[ziglings](https://codeberg.org/ziglings/exercises/) — Zig version of Rustlings, worth checking out if you're curious about Zig.
