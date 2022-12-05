# [animation_model](animation_model.hpp)

Represents an animation as loaded from the disk. This is then used as a [flyweight](https://en.wikipedia.org/wiki/Flyweight_pattern) to play [animations](animation.md).

This base class only contains common attributes, while the actual data is held in derived classes such as [assimp_animation_model](assimp/assimp_animation_model.md).

## Members

### tick

```cpp
virtual void tick(float delta_time, struct animated_object & object) const noexcept = 0;
```

Advances `object`'s [animation](animation.md) by `delta_time`.

### name

```cpp
std::string name;
```

The animation's name.

### total_time

```cpp
float total_time = 0.f;
```

The animation's duration.

### ticks_per_second

```cpp
float ticks_per_second = 0.f;
```

The number of ticks per second when playing the animation.