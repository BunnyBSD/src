{ pkgs ? import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/531670d871c0.tar.gz";
  }) {}
}:

let
  hostIncludes = "-I${pkgs.zlib.dev}/include -I${pkgs.ncurses.dev}/include";
  hostLibs     = "-L${pkgs.zlib.out}/lib -L${pkgs.ncurses.out}/lib";
in
(pkgs.mkShell.override { stdenv = pkgs.gcc13Stdenv; }) {
  # disable nix's hardening because of old 2004 groff
  hardeningDisable = [ "format" ];

  nativeBuildInputs = with pkgs; [
    bmake
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
