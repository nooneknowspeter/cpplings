{
  description = "cpplings flake";

  inputs = {
    flake-utils = {
      url = "github:numtide/flake-utils";
    };

    nixpkgs = {
      url = "github:nixos/nixpkgs/nixpkgs-unstable";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;

          config = {
            allowUnfree = true;
          };
        };

        fastfetch_cmd = "cat .ascii-art.txt | fastfetch --raw - --logo-width 15 --logo-height 7 --logo-padding-right 25 --logo-padding-top 10";

        fmt_pkgs = with pkgs; [
          treefmt

          # formatters & linters
          actionlint
          bash-language-server
          beautysh
          clang-tools
          deadnix
          dockerfile-language-server
          marksman
          nil
          nixd
          nixfmt
          prettier
          shellcheck
          statix
          taplo
          yaml-language-server
          zls
        ];

        runtime_pkgs = with pkgs; [
          # compilers & runtimes
          clang
          gcc
          nodejs
          python3
          stdenv.cc.cc
          zig
        ];

        dev_pkgs = with pkgs; [
          # dev tools
          diffutils
          fastfetch
          gdb
          gnumake
          hyperfine
          just
          lldb
          patch
          perf
          valgrind
          opencode
        ];

        doc_pkgs = with pkgs; [
          # docs
          pandoc
          quarto
        ];

        ld_pkgs = with pkgs; [
          stdenv.cc.cc.lib
        ];
      in
      {
        devShells = {
          default = pkgs.mkShell {
            packages = [
            ]
            ++ fmt_pkgs
            ++ runtime_pkgs
            ++ dev_pkgs
            ++ doc_pkgs;

            shellHook = ''
              ${fastfetch_cmd}
            '';

            LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath ld_pkgs}:$LD_LIBRARY_PATH";
          };

          fmt = pkgs.mkShell {
            packages =
              with pkgs;
              [
                just
              ]
              ++ fmt_pkgs;

            shellHook = ''
              ${fastfetch_cmd}
            '';
          };
        };
      }
    );
}
