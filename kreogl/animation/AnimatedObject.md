# [AnimatedObject](AnimatedObject.hpp)

An [Object](../Object.md) on which [Animations](Animation.md) can be played. The object's [model](../model/Model.md) should always be an [AnimatedModel](AnimatedModel.md).

## Members

### skeleton

```cpp
Skeleton skeleton;
```

This object's [Skeleton](Skeleton.md).

### animation

```cpp
std::optional<Animation> animation;
```

The [Animation](Animation.md) being played.

### tickAnimation

```cpp
void tickAnimation(float deltaTime) noexcept;
```

Advances the animation by `deltaTime`, updating the bones in `skeleton` (and possibly this object's `transform`, depending on the mover behaviors).