{ nixpkgs ? import <nixpkgs> {} }:

let
    stdenv = nixpkgs.overrideCC nixpkgs.stdenv nixpkgs.clang;
in
    stdenv.mkDerivation {
        name = "my";

        src = ./.;

        buildInputs = with nixpkgs; [
            pkg-config
            (lib.getLib libftdi)
            (lib.getDev libftdi)
            (lib.getLib gtest)
            (lib.getDev gtest)
            (lib.getDev magic-enum)
        ];

        doCheck = true;
    }
