This repo contains a solver for the IQ twist puzzle game [1]. It uses an implementation of the dancing links algorithm [here](https://github.com/Logismos/dlx). The solver compiles to webassembly, and a very simple front-end has been made for generating and visualizing solutions.

1. https://www.smartgames.eu/uk/one-player-games/iq-twist

## Prerequisites

[Emscripten](https://emscripten.org/docs/getting_started/downloads.html) and python3.

## Usage

The following command generates the appropriate webassembly bindings and runs a local server:
```
make run
```

See live demo (TODO).

## Notes
When cloning, don't forget to add the recursive option to capture the submodule:
```
git clone --recursive https://github.com/Logismos/iq-twist.git
```

