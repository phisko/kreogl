# [animation](animation.hpp)

Holds information required to play an animation on an [animated_object](animated_object.md).

## Members

### model

```cpp
const animation_model * model = nullptr;
```

The [model](animation_model.md) for this animation.

### current_time

```cpp
float current_time = 0.f;
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

### mover_behavior enum

```cpp
enum class mover_behavior {
	update_transform,
	update_bones,
	none
};
```

Determines the effect of the animation's "mover" (its first node that affects bones).

* `update_transform`: will update the [object](../object.md)'s `transform`
* `update_bones`: will update the `object`'s [skeleton](skeleton.md)
* `none`: will not update anything

`update_transform` is useful when playing movement animations, such as a run or turn animation, which will then naturally affect the object.

### Mover behaviors

```cpp
mover_behavior position_mover_behavior = mover_behavior::update_bones;
mover_behavior rotation_mover_behavior = mover_behavior::update_bones;
```

Determine the effects to be applied to the `object`'s position and rotation, respectively.

### movement_up_to_last_frame

```cpp
struct movement_up_to_frame {
	glm::vec3 position{ 0.f };
	glm::quat rotation{ 0.f, 0.f, 0.f, 0.f };
	glm::vec3 scale{ 1.f };
};
movement_up_to_frame movement_up_to_last_frame;
```

Used internally.