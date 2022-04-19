# kreogl

[![build](https://github.com/phisko/kreogl/workflows/build/badge.svg)](https://github.com/phisko/kreogl/actions/workflows/build.yml)

A simple OpenGL renderer, aimed to get any game project up and running with simple but satisfying graphics.

## Motivation

I'm not a graphics programmer and, although I do find it fun when I get pretty things to appear on-screen, I much prefer [metaprogramming](https://github.com/phisko/reflection/) and [engine architecture](https://github.com/phisko/kengine/). However, whenever I've wanted to get some proof-of-concept graphics, I've always been frustrated to see there's no 3D equivalent to [SFML](https://www.sfml-dev.org/): a simple 3D graphics library, letting me load meshes and draw them without having to write my own shaders.

Eventually I came to accept this, and started implementing a custom OpenGL system for the [kengine](https://github.com/phisko/kengine/). This was a learning process for me, as it was my first time toying with graphics programming.

Now that I'm relatively satisfied with the final result (although it's far from perfect), I figured it was time to move the functionality out of the `kengine` and have it become what I was initially looking for, so that future engine programmers don't have to write their own renderer from scratch.
