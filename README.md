# kreogl

[![build](https://github.com/phisko/kreogl/workflows/build/badge.svg)](https://github.com/phisko/kreogl/actions/workflows/build.yml)

A simple OpenGL renderer, aimed to get any game project up and running with simple but satisfying graphics.

## Motivation

I'm not a graphics programmer and, although I do find it fun when I get pretty things to appear on-screen, I much prefer [metaprogramming](https://github.com/phisko/reflection/) and [engine architecture](https://github.com/phisko/kengine/). However, whenever I've wanted to get some proof-of-concept graphics, I've always been frustrated to see there's no 3D equivalent to [SFML](https://www.sfml-dev.org/): a simple 3D graphics library, letting me load meshes and draw them without having to write my own shaders.

Eventually I came to accept this, and started implementing a custom OpenGL system for the [kengine](https://github.com/phisko/kengine/). This was a learning process for me, as it was my first time toying with graphics programming.

Now that I'm relatively satisfied with the final result (although it's far from perfect), I figured it was time to move the functionality out of the `kengine` and have it become what I was initially looking for, so that future engine programmers don't have to write their own renderer from scratch.

## Usage

Take a look at the [example code](example/main.cpp), which makes use of most available features.

## API

### High level objects

These are objects that will typically live for as long as the application is running.

* [Window](kreogl/Window.md)
* [World](kreogl/World.md)
* [Camera](kreogl/Camera.md)

### Basic drawables

* [Object](kreogl/Object.md)
* [Sprite](kreogl/Sprite.md)
* [Text](kreogl/Text.md)
* [DebugElement](kreogl/DebugElement.md)

### Lights

* [DirectionalLight](kreogl/lights/DirectionalLight.md)
* [PointLight](kreogl/lights/PointLight.md)
* [SpotLight](kreogl/lights/SpotLight.md)

### Loaders

These provide functions to load [Models](kreogl/model/Model.md) from files.

* [AssImp](kreogl/loaders/assimp/AssImp.md)
* [PolyVox](kreogl/loaders/polyvox/PolyVox.md)

### Animation

* [AnimatedObject](kreogl/animation/AnimatedObject.md)

The rest of these types are less user-facing, and understanding them isn't required for basic animation code.

* [AnimatedModel](kreogl/animation/AnimatedModel.md)
* [Animation](kreogl/animation/Animation.md)
* [AnimationFile](kreogl/animation/AnimationFile.md)
* [AnimationModel](kreogl/animation/AnimationModel.md)
* [Skeleton](kreogl/animation/Skeleton.md)
* [SkeletonModel](kreogl/animation/SkeletonModel.md)

## Implementation details

This describe the internal implementation of the rendering engine. You don't need to be aware of these to make use of `kreogl`, but if you wish to improve/extend/understand its behavior, this is a good starting point.

### [Viewport](kreogl/impl/Viewport.md)

Each [Camera](kreogl/Camera.md) has an associated viewport, which represents the on-screen area used to display the camera.

### [GBuffer](kreogl/impl/GBuffer.md)

Each [Viewport](kreogl/impl/Viewport.md) has an underlying G-buffer, which contains the intermediate rendering data generated when drawing the camera content. It can be used to query the position, color, or custom user data that was drawn in a specific pixel.

### [RAII](https://en.cppreference.com/w/cpp/language/raii) wrappers to OpenGL resources

* [Buffer](kreogl/impl/RAII/Buffer.md)
* [FrameBuffer](kreogl/impl/RAII/FrameBuffer.md)
* [ScopedBindFramebuffer](kreogl/impl/RAII/ScopedBindFramebuffer.md)
* [ScopedGLFeature](kreogl/impl/RAII/ScopedGLFeature.md)
* [Texture](kreogl/impl/RAII/Texture.md)
* [VertexArray](kreogl/impl/RAII/VertexArray.md)

### Shadow maps

* [ShadowMap](kreogl/impl/shadowMaps/ShadowMap.md)
* [ShadowCube](kreogl/impl/shadowMaps/ShadowCube.md)
* [CascadedShadowMap](kreogl/impl/shadowMaps/CascadedShadowMap.md)

### Textures

* [ImageTexture](kreogl/impl/texture/ImageTexture.md)
* [SkyboxTexture](kreogl/impl/texture/SkyboxTexture.md)

### Shapes

* [Box](kreogl/impl/shapes/Box.md)
* [Line](kreogl/impl/shapes/Line.md)
* [Quad](kreogl/impl/shapes/Quad.md)
* [Sphere](kreogl/impl/shapes/Sphere.md)
* [TexturedQuad](kreogl/impl/shapes/TexturedQuad.md)

### Shaders

Shaders are instances of [Shader](kreogl/impl/shaders/Shader.md), grouped into a [ShaderPipeline](kreogl/impl/shaders/ShaderPipeline.md), which can be passed to `Window::draw`. The default pipeline contains all the pre-implemented shaders:

#### GBuffer shaders

These are shaders in charge of filling the [GBuffer](kreogl/impl/GBuffer.md).

* [PositionColorShader](kreogl/impl/shaders/gbuffer/PositionColor/PositionColorShader.md)
* [SkeletalTexturedShader](kreogl/impl/shaders/gbuffer/SkeletalTextured/SkeletalTexturedShader.md)
* [DebugShader](kreogl/impl/shaders/gbuffer/Debug/DebugShader.md)
* [SpriteShader](kreogl/impl/shaders/gbuffer/Sprite/SpriteShader.md)
* [TextShader](kreogl/impl/shaders/gbuffer/Text/TextShader.md)

#### Lighting shaders

These are shaders in charge of applying lighting to what was previously written into the [GBuffer](kreogl/impl/GBuffer.md), and writing the result to the main framebuffer.

* [DirectionalLightShader](kreogl/impl/shaders/lighting/DirectionalLight/DirectionalLightShader.md)
* [PointLightShader](kreogl/impl/shaders/lighting/PointLight/PointLightShader.md)
* [SpotLightShader](kreogl/impl/shaders/lighting/SpotLight/SpotLightShader.md)

#### Post-lighting shaders

These are shaders that run after the lighting pass, and can render effects to alter the lighting of the scene.

* [VolumetricDirectionalLight](kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricDirectionalLight/VolumetricDirectionalLightShader.md)
* [VolumetricPointLight](kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricPointLight/VolumetricPointLightShader.md)
* [VolumetricSpotLight](kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricSpotLight/VolumetricSpotLightShader.md)

#### Post-process shaders

These are shaders that run after the post-lighting pass, and can add the "final touches" to the scene.

* [LightSphereShader](kreogl/impl/shaders/postProcess/LightSphere/LightSphereShader.md)
* [SkyboxShader](kreogl/impl/shaders/postProcess/Skybox/SkyboxShader.md)

#### Shadowmap shaders

These are shaders that implement the [ShadowMapShader](kreogl/impl/shaders/shadowMap/ShadowMapShader.md) or [ShadowCubeShader](kreogl/impl/shaders/shadowMap/ShadowCubeShader.md) interfaces, and are called by the lighting shaders to fill lights' [ShadowMaps](kreogl/impl/shadowMaps/ShadowMap.md).

* [PositionShadowMapShader](kreogl/impl/shaders/shadowMap/PositionShadowMap/PositionShadowMapShader.md)
* [PositionShadowCubeShader](kreogl/impl/shaders/shadowMap/PositionShadowCube/PositionShadowCubeShader.md)
* [SkeletalShadowMapShader](kreogl/impl/shaders/shadowMap/SkeletalShadowMap/SkeletalShadowMapShader.md)
* [SkeletalShadowCubeShader](kreogl/impl/shaders/shadowMap/SkeletalShadowCube/SkeletalShadowCubeShader.md)