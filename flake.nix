{
  description = "「踏めば助かるのに...」でおなじみのロボカスに何でも言わせる";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } (_: {
      systems =  [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      perSystem = { self', pkgs, lib, ... }: {
        packages.robokssay = pkgs.stdenv.mkDerivation {
          name = "robokssay";
          src = lib.cleanSource ./.;
          buildPhase = ''
            $CC main.c -o robokssay
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp robokssay $out/bin
          '';
        };
        apps.robokssay = {
          type = "app";
          program = "${self'.packages.robokssay}/bin/robokssay";
        };
      };
    });
}
