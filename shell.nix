{ pkgs ? import <nixpkgs> {} }:

let
  hostIncludes = "-I${pkgs.zlib.dev}/include -I${pkgs.ncurses.dev}/include";
  hostLibs     = "-L${pkgs.zlib.out}/lib -L${pkgs.ncurses.out}/lib";
in
(pkgs.mkShell.override { stdenv = pkgs.gcc13Stdenv; }) {
  # disable nix's hardening because of old 2004 groff
  hardeningDisable = [ "format" ];

  nativeBuildInputs = with pkgs; [
    gnumake
    clang-tools
    flex
    bison
    texinfo
    perl
    git
    rsync
    patch
    file
    bc
    bear
  ];

  buildInputs = with pkgs; [
    zlib
    ncurses
    openssl
  ];

  # ugly hack but works.
  HOST_CC  = "gcc ${hostIncludes}";
  HOST_CXX = "g++ ${hostIncludes}";
  HOST_CFLAGS = "-Wno-error=format-security";
  HOST_CXXFLAGS = "-Wno-error=format-security";
  HOST_LDFLAGS = "${hostLibs}";
}
