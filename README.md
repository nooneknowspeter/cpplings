# cpplings

**cpplings**, inspired by [Rustlings](https://github.com/rust-lang/rustlings),
is a set of hands-on exercises to help you get comfortable reading
and writing modern **C++**.

These exercises reference the excellent [learncpp](https://www.learncpp.com/)
and are conceptually related to [cplings](https://github.com/rdjondo/cplings),
which adopted this idea earlier.

We recommend doing these exercises alongside reading [learncpp](https://www.learncpp.com/)
and checking out the additional learning resources listed below
and in the comments of the source code.

---

## Why Zig?

This project uses **[Zig](https://ziglang.org/)** as a toolchain instead of CMake
or any other build system used for C++ development.
This is a deliberate choice to test Zig’s practical usefulness in real-world C++ workflows.

### Benefits of Using Zig

- Zig is simple but powerful.
- It ships with a Clang-based compiler toolchain built in.
- It simplifies compiling for multiple architectures (x86, ARM, etc.).
- It removes the pain of installing and managing compiler versions manually (GCC, Clang, etc.).
- Zig-based builds are highly reproducible and consistent across platforms.

> [!NOTE]
>
> Although Nix technically solves the problem with managing dependencies and
> their versions, I strongly support using Zig over CMake
> as a toolchain for its vector processing (building wth SIMD)
> and for being an elegant and minimalistic language.

---

## Build & Setup

Start by cloning this repository on your machine.

> [!NOTE]
>
> If you want to store and save your progress you can fork the repository or
> change the remote orgin URI to your own repository

This project uses a [`Makefile`](./Makefile) to simplify common development tasks.
You will need `make` installed on your system.
If you do not want to install make (make is provided in the Nix shell environment)
or don't have it installed, you can use the commands specified in the [`Makefile`](./Makefile).

| Command           | Description                                                                                           |
| ----------------- | ----------------------------------------------------------------------------------------------------- |
| make build        | Builds the project using `zig build`, the executable can be found in `./zig-out/bin/cpplings`         |
| make run          | Build the project like `zig build` then runs the executable                                           |
| make nix          | Starts the Nix development environment                                                                |
| make docker-build | Builds a local Docker image                                                                           |
| make docker-run   | Starts a container using the `cpplings` image and mounts a volume to the path of the clone repository |
| make docker-exec  | Executes and enters the bash shell of the container and `/cpplings` directory                         |
| make docker-stop  | Stops the container                                                                                   |
| make docker-rm    | Removed the container                                                                                 |

### Native Package Manager

If you would like to compile using your own package manager; `homebrew`, `pacman`, `apt`, etc.
You can install the following:

- clang-tools (clang-format)
- gdb/lldb (debugging)
- zig

### asdf-vm

If you're using [`asdf`](https://github.com/asdf-vm/asdf), you can install the dependencies using:

```sh
asdf install
```

Check [`.tool-versions`](./.tool-versions) and ensure you have the plugins added such as:

```sh
asdf plugin add <plugin-name>
```

### Nix

If you're using NixOS or Nix installed, a flake based nix shell environment is provided with all required dependencies,
such as Zig, and more.

> [!NOTE]
>
> If you do not have Nix installed, install it [here](https://nixos.org/download/).
>
> Ensure that you install the multi-user version of Nix

> [!NOTE]
>
> If you do not want to install Nix on your system but have Docker installed and running,
> you can use the [Docker approach](#docker) which uses an Arch Linux container with Nix installed.

Start the environment with:

> [!NOTE]
>
> Ensure you have extra-experimental features on with `experimental-features = nix-command flakes`
> in your `/etc/nix/nix.conf` or `~/.config/nix/nix.conf`
>
> or
>
> use it inline `nix --extra-experimental-features "nix-command flakes" develop`

```sh
nix develop
```

This gives you a fully configured development shell without installing the dependencies globally on your system.

> [!NOTE]
>
> A `.envrc` is provided to allow the nix shell to be used automatically
>
> Make sure to have `direnv` installed and use `direnv allow` when in the project directory
>
> If it gets a little annoying, you can use `direnv disallow` to disable it and manually enter
> the shell with `nix develop`

If you are unfarmiliar with Nix,
it is a package manager and system configuration tool.
It prevents the "it works on my machine" problem by allowing declerative and reproducible builds of environments.

You can read more about it [here](https://nixos.org/guides/how-nix-works/)
and the extra experimental features (flakes) [here](https://nixos-and-flakes.thiscute.world/nixos-with-flakes/introduction-to-flakes).

### Docker

A `Dockerfile` is provided for containerized development.

The container is based off the Arch Linux image with a multi-user version of Nix installed.

The container is purely made for compiling, it uses a shared volume with the cpplings folder on your system.
This allows coding and editing to be done in a decoupled/modular way.
It can be done directly on your machine in the cpplings directory with the installed development
tools on your system or another environment (Code editor running in another container or some sort of system).

You can build and run the container locally using:

```sh
make docker-build && \
make docker-run
```

The image will be tagged as `cpplings`.

The container mounts your current project directory into `/cpplings` in the container.

This setup is ideal for isolated or reproducible builds.

A pre-built image is also available via GitHub Container Registry by running:

```sh
make docker-run
```

> [!NOTE]
>
> If you would like to set up an environment as quick as possible and haven't cloned this repository, you can use:
>
> #### Unix
>
> ```sh
> # change CPPLINGS_DIR to your liking \
> # example: ~/projects/cpplings or ~/git/cpplings \
> export CPPLINGS_DIR=~/cpplings && \
> export CPPLINGS_OCI_IMAGE_URL=ghcr.io/nooneknowspeter/cpplings:main && \
>
> git clone https://github.com/nooneknowspeter/cpplings.git ${CPPLINGS_DIR} && \
> cd ${CPPLINGS_DIR} && \
>
> docker run -itd -v .:/cpplings --name cpplings ${CPPLINGS_OCI_IMAGE_URL} && \
>
> # uncomment the line below if you're using VS Code \
> # code . -r && \
> # or replace command with editor of choice \
> # nvim . && \
>
> # execute the line below seperately (editor's terminal or seperate terminal instance)
> # if you are running the editor command above
> docker exec -it -w "/cpplings" cpplings bash
> ```
>
> #### Windows
>
> ```ps1
> # change CPPLINGS_DIR to your liking \
> # example: $HOME\projects\cpplings or $HOME\git\cpplings \
> $env:CPPLINGS_DIR = "$HOME\cpplings"; \
> $env:CPPLINGS_OCI_IMAGE_URL = "ghcr.io/nooneknowspeter/cpplings:main"; \
>
> git clone https://github.com/nooneknowspeter/cpplings.git $env:CPPLINGS_DIR; \
> cd $env:CPPLINGS_DIR; \
>
> docker run -itd -v .:/cpplings --name cpplings $env:CPPLINGS_OCI_IMAGE_URL;
>
> # uncomment the line below if you're using VS Code \
> # code . -r; \
> # or replace command with editor of choice \
> # nvim .; \
>
> # execute the line below seperately (editor's terminal or seperate terminal instance)
> # if you are running the editor command above
> docker exec -it -w "/cpplings" cpplings bash
> ```
>
> Copy and paste, then edit to your liking.
>
> This will launch the shell of the container instantly, use it in your VS Code terminal,
> or a split pane if you're using terminal multiplexer, or just your terminal.

> [!NOTE]
>
> Run the container's shell using:
>
> ```sh
> docker exec -it -w "/cpplings" cpplings bash
> ```

> [!NOTE]
>
> Using `nix develop` will result in an ownership problem
>
> Inside the docker container's shell run:
>
> ```sh
> chown -R root:users /cpplings && \
> chmod a+rwX /cpplings
> ```
>
> Enter inside the nix development shell using:
>
> ```sh
> nix develop
> ```

> [!NOTE]
>
> You may encounter a problem with not being able to execute a few commands such as `git` on your local machine
> because the owner of the folder will be root
>
> You will have to change the owner to your local machine's user `chown -R <username> /path/to/cpplings`

### Development Environment

#### VS Code

For VS Code users who are unfirmiliar with C++, an [`extensions.json`](./.vscode/extensions.json) file is included.
It has the recommended extensions for writing and debugging C++, and the other tools in this project.

#### Neovim

For Neovim users using Nix as their package manager (NixCats, etc.),
can use `nix develop` for a nix dev shell with all project related
LSPs, DAPs, formatters and linters.

If you're on NixOS and still use Mason as a package manager for Neovim LSPs and so on,
you can still use this shell to install the required packages (clangd, zsl, ...) through Mason.

You can use whichever tools you prefer and may want to try.

Have fun.

---

## Resources

[learncpp](https://www.learncpp.com/) — Comprehensive and beginner-friendly C++ guide.

[cppreference](https://en.cppreference.com/w/) — The canonical C++ reference.

[cppbyexample](https://cppbyexample.com/) — C++ concepts via concise examples.

[learnxinyminutes: C++](https://learnxinyminutes.com/docs/c++) — Fast overview for experienced programmers.

[zig's build system](https://ziglang.org/learn/build-system/) — Documentation on Zig's build system

[ziglings](https://codeberg.org/ziglings/exercises/) — Zig version of Rustlings, worth checking out if you're curious about Zig.

[Maintain It With Zig](https://kristoff.it/blog/maintain-it-with-zig/) — A well written article by Loris Cro,
explaining the benefits of Zig's build system in the world of C/C++

[Why Zig When There is Already C++, D, and Rust?](https://ziglang.org/learn/why_zig_rust_d_cpp/) — Another article on the benefits of Zig

[allyourcodebase](https://github.com/allyourcodebase) - C/C++ projects built with Zig
