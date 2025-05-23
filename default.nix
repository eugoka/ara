{ nixpkgs ? import <nixpkgs> {} }:

let
    stdenv = nixpkgs.overrideCC nixpkgs.stdenv nixpkgs.clang;
in
    stdenv.mkDerivation {
        name = "my";

        src = ./.;

        buildInputs = with nixpkgs; [
            pkg-config
            (lib.getLib libusb1)
            (lib.getLib libftdi1)
            (lib.getDev libftdi1)
            (lib.getLib gtest)
            (lib.getDev gtest)
            (lib.getDev magic-enum)
        ];

        doCheck = true;
    }
