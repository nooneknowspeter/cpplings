{
  description = "Development environment for cpplings";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f: nixpkgs.lib.genAttrs supportedSystems (system: f { pkgs = import nixpkgs { inherit system; }; });
    in
    {

      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              # formatters & linters
              actionlint
              bash-language-server
              beautysh
              clang-tools
              deadnix
              dockerfile-language-server-nodejs
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

              # compilers & runtimes
              clang
              gcc
              nodejs
              python3
              stdenv.cc.cc
              zig

              # dev tools
              fastfetch
              gdb
              gnumake
              lldb
              treefmt
            ];

            shellHook = ''
              							export LD_LIBRARY_PATH="${pkgs.stdenv.cc.cc.lib}/lib:$LD_LIBRARY_PATH"
                            cat .ascii-art.txt | fastfetch --raw - --logo-width 15 --logo-height 7 --logo-padding-right 25 --logo-padding-top 10
            '';

          };
        }
      );
    };
}
