# [AnimationModel](AnimationModel.hpp)

Represents an animation as loaded from the disk. This is then used as a [flyweight](https://en.wikipedia.org/wiki/Flyweight_pattern) to play [Animations](Animation.md).

This base class only contains common attributes, while the actual data is held in derived classes such as [AssImpAnimationModel](AssImp/AssImpAnimationModel.md).

## Members

### tick

```cpp
virtual void tick(float deltaTime, struct AnimatedObject & object) const noexcept = 0;
```

Advances `object`'s [Animation](Animation.md) by `deltaTime`.

### name

```cpp
std::string name;
```

The animation's name.

### totalTime

```cpp
float totalTime = 0.f;
```

The animation's duration.

### ticksPerSecond

```cpp
float ticksPerSecond = 0.f;
```

The number of ticks per second when playing the animation.