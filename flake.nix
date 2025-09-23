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
              actionlint
              bash-language-server
              beautysh
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

              clang
              clang-tools
              gcc
              lldb
              nodejs
              python3

              fastfetch
              gnumake
              treefmt
              zig
            ];

            shellHook = ''
              cat .ascii-art.txt | fastfetch --raw - --logo-width 15 --logo-height 7 --logo-padding-right 25 --logo-padding-top 10
            '';
          };
        }
      );
    };
}
