# [Animation](Animation.hpp)

Holds information required to play an animation on an [AnimatedObject](AnimatedObject.md).

## Members

### model

```cpp
const AnimationModel * model = nullptr;
```

The [model](AnimationModel.md) for this animation.

### currentTime

```cpp
float currentTime = 0.f;
```

Tracks how far we've progressed in the animation.

### speed

```cpp
float speed = 1.f;
```

Controls animation playback speed.

### loop

```cpp
bool loop = false;
```

Controls whether the animation should loop.

### MoverBehavior enum

```cpp
enum class MoverBehavior {
	UpdateTransform,
	UpdateBones,
	None
};
```

Determines the effect of the animation's "mover" (its first node that affects bones).

* `UpdateTransform`: will update the [Object](../Object.md)'s `transform`
* `UpdateBones`: will update the `Object`'s [Skeleton](Skeleton.md)
* `None`: will not update anything

`UpdateTransform` is useful when playing movement animations, such as a run or turn animation, which will then naturally affect the object.

### Mover behaviors

```cpp
MoverBehavior positionMoverBehavior = MoverBehavior::UpdateBones;
MoverBehavior rotationMoverBehavior = MoverBehavior::UpdateBones;
```

Determine the effects to be applied to the `Object`'s position and rotation, respectively.

### movementUpToLastFrame

```cpp
struct MovementUpToFrame {
	glm::vec3 position{ 0.f };
	glm::quat rotation{ 0.f, 0.f, 0.f, 0.f };
	glm::vec3 scale{ 1.f };
};
MovementUpToFrame movementUpToLastFrame;
```

Used internally.