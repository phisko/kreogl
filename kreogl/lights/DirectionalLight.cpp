#include "DirectionalLight.hpp"

// stl
#include <array>

// kreogl
#include "kreogl/Camera.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	static glm::vec3 getCorrectDirection(const glm::vec3 & dir) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto normalized = glm::normalize(dir);
		return normalized.y == 1.f ?
			glm::vec3(.001f, .999f, 0.f) :
			normalized.y == -1.f ?
			glm::vec3(.001f, -.999f, 0.f) :
			dir;
	}

	struct CascadeBounds {
		static constexpr auto cornerCount = 8;
		std::array<glm::vec4, cornerCount> corners;
		glm::vec3 center;
	};

	static CascadeBounds getCascadeBoundsWorldSpace(const glm::mat4 & proj, const glm::mat4 & view) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto inv = glm::inverse(proj * view);

		const glm::vec3 ndcCorners[] = {
			{ -1.f, -1.f, -1.f }, // left bottom back
			{ 1.f, -1.f, -1.f }, // right bottom back
			{ -1.f, 1.f, -1.f, }, // left top back
			{ 1.f, 1.f, -1.f }, // right top back
			{ -1.f, -1.f, 1.f }, // left bottom front
			{ 1.f, -1.f, 1.f }, // right bottom front
			{ -1.f, 1.f, 1.f }, // left top front
			{ 1.f, 1.f, 1.f }, // right top front
		};
		static_assert(std::extent_v<decltype(ndcCorners)> == CascadeBounds::cornerCount);

		CascadeBounds bounds;
		for (unsigned int i = 0; i < CascadeBounds::cornerCount; ++i) {
			const auto pt = inv * glm::vec4(ndcCorners[i], 1.f);
			bounds.corners[i] = pt / pt.w;
		}

		bounds.center = glm::vec3(0, 0, 0);
		for (const auto & v : bounds.corners)
			bounds.center += glm::vec3(v);
		bounds.center /= CascadeBounds::cornerCount;

		return bounds;
	}

	// based on https://learnopengl.com/Guest-Articles/2021/CSM
	glm::mat4 DirectionalLight::getLightSpaceMatrixForCascade(const DrawParams & params, size_t index) const noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto nearPlane = index == 0 ? params.camera.getNearPlane() : std::max(params.camera.getNearPlane(), cascadeEnds[index - 1]);
		const auto farPlane = std::min(params.camera.getFarPlane(), cascadeEnds[index]);

		const auto proj = glm::perspective(params.camera.getFOV(), params.camera.getViewport().getAspectRatio(), nearPlane, farPlane);

		const auto cascadeBoundsWorldSpace = getCascadeBoundsWorldSpace(proj, params.camera.getViewMatrix());

		const auto dir = getCorrectDirection(direction);
		const auto lightView = glm::lookAt(cascadeBoundsWorldSpace.center - glm::normalize(dir), cascadeBoundsWorldSpace.center, { 0.f, 1.f, 0.f });

		glm::vec3 min(std::numeric_limits<float>::max());
		glm::vec3 max(std::numeric_limits<float>::min());

		for (const auto & worldPos : cascadeBoundsWorldSpace.corners) {
			const auto lightPos = lightView * worldPos;
			min = glm::min(min, glm::vec3(lightPos));
			max = glm::max(max, glm::vec3(lightPos));
		}

		const auto largestExtent = glm::max(glm::abs(min), glm::abs(max));
		const auto lightProj = glm::ortho(-largestExtent.x, largestExtent.x, -largestExtent.y, largestExtent.y, -largestExtent.z - shadowCasterMaxDistance, largestExtent.z + shadowCasterMaxDistance);

		return lightProj * lightView;
	}
}