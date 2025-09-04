{
  description = "Cardage :: Development Enviroment";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";

  outputs = inputs: let
    supportedSystems = [
      "x86_64-linux"
      "aarch64-linux"
      "x86_64-darwin"
      "aarch64-darwin"
    ];
    forEachSupportedSystem = f:
      inputs.nixpkgs.lib.genAttrs supportedSystems (
        system:
          f {
            pkgs = import inputs.nixpkgs {inherit system;};
          }
      );
  in {
    devShells = forEachSupportedSystem (
      {pkgs}: {
        default =
          pkgs.mkShell.override
          {
            # Override stdenv in order to change compiler:
            # stdenv = pkgs.clangStdenv;
          }
          {
            packages = with pkgs;
              [
                # Raylib's Dependencies
                libGL

                xorg.libX11
                xorg.libX11.dev
                xorg.libXcursor
                xorg.libXi
                xorg.libXinerama
                xorg.libXrandr

                emscripten

                # C/C++ Tools
                bear
                clang-tools
              ]
              ++ (
                if system == "aarch64-darwin"
                then []
                else [gdb]
              );
          };
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [pkgs.alsa-lib];
      }
    );
  };
}
