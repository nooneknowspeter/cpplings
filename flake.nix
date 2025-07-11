{
  description = "Development environment for cpplings";

  inputs = { nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable"; };

  outputs = { self, nixpkgs }:
    let
      supportedSystems =
        [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forEachSupportedSystem = f:
        nixpkgs.lib.genAttrs supportedSystems
          (system: f { pkgs = import nixpkgs { inherit system; }; });
    in
    {

      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell {
          packages = with pkgs; [ clang-tools fastfetch gnumake lldb zig ];

          shellHook = ''
            cat .ascii-art.txt | fastfetch --raw - --logo-width 15 --logo-height 7 --logo-padding-right 25 --logo-padding-top 10
          '';
        };

        editor = pkgs.mkShell {
          packages = with pkgs; [
            marksman
            actionlint
            bash-language-server
            clang-tools
            dockerfile-language-server-nodejs
            nil
            nixd
            nixfmt
            nodejs
            python3
            statix
            yaml-language-server
            zls
          ];
        };
      });

    };
}
