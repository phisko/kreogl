# [animated_object](animated_object.hpp)

An [object](../object.md) on which [animations](animation.md) can be played. The object's [model](../model/model.md) should always be an [animated_model](animated_model.md).

## Members

### skeleton

```cpp
skeleton skeleton;
```

This object's [skeleton](skeleton.md).

### animation

```cpp
std::optional<animation> animation;
```

The [animation](animation.md) being played.

### tick_animation

```cpp
void tick_animation(float delta_time) noexcept;
```

Advances the animation by `delta_time`, updating the bones in `skeleton` (and possibly this object's `transform`, depending on the mover behaviors).