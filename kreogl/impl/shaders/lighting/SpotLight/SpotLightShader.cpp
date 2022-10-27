#include "SpotLightShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/shapes/Sphere.hpp"
#include "kreogl/impl/GBuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/shaders/ShaderPipeline.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowMapShader.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	SpotLightShader::SpotLightShader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("SpotLightShader");

		useWithoutUniformCheck();

		_glsl.gposition = (int)GBuffer::Texture::Position;
		_glsl.gnormal = (int)GBuffer::Texture::Normal;
		_glsl.gdiffuse = (int)GBuffer::Texture::DiffuseAndShouldIgnoreLighting;
		_glsl.gspecular = (int)GBuffer::Texture::Specular;

		_shadowMapGLSL.shadowMap = (int)GBuffer::Texture::Count;
	}

	void SpotLightShader::addSourceFiles() noexcept {
		KREOGL_PROFILING_SCOPE;

		addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
		addSourceFile(SampleShadowMapGLSL::frag, GL_FRAGMENT_SHADER);
		addSourceFile(SpotLightGLSL::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<UniformBase *> SpotLightShader::getUniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			// PositionProjViewModelGLSL
			&_ppvmGLSL.proj,
			&_ppvmGLSL.view,
			&_ppvmGLSL.model,
			// SpotLightGLSL
			&_glsl.gposition,
			&_glsl.gnormal,
			&_glsl.gdiffuse,
			&_glsl.gspecular,
			&_glsl.viewPos,
			&_glsl.screenSize,
			&_glsl.color,
			&_glsl.position,
			&_glsl.direction,
			&_glsl.cutOff,
			&_glsl.outerCutOff,
			&_glsl.diffuseStrength,
			&_glsl.specularStrength,
			&_glsl.attenuationConstant,
			&_glsl.attenuationLinear,
			&_glsl.attenuationQuadratic,
			// SampleShadowMapGLSL
			&_shadowMapGLSL.shadowMap,
			&_shadowMapGLSL.lightSpaceMatrix,
			&_shadowMapGLSL.minBias,
			&_shadowMapGLSL.maxBias,
			&_shadowMapGLSL.pcfSamples
		};
	}

	void SpotLightShader::draw(const DrawParams & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		useWithoutUniformCheck();

		const ScopedGLFeature cull(GL_CULL_FACE);
		const ScopedGLFeature blend(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_glsl.viewPos = params.camera.getPosition();
		_glsl.screenSize = params.camera.getViewport().getResolution();

		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count));

		for (const auto light : params.world.getSpotLights()) {
			if (light->castShadows)
				updateShadowMap(*light, params);

			const auto radius = light->getRadius();
			if (glm::length(params.camera.getPosition() - light->position) < radius)
				glCullFace(GL_BACK);
			else
				glCullFace(GL_FRONT);

			const auto uniformChecker = use();

			_ppvmGLSL.proj = params.camera.getProjMatrix();
			_ppvmGLSL.view = params.camera.getViewMatrix();

			glm::mat4 model(1.f);
			model = glm::translate(model, light->position);
			model = glm::scale(model, glm::vec3(radius));
			_ppvmGLSL.model = model;

			_glsl.color = light->color;
			_glsl.position = light->position;
			_glsl.direction = light->direction;
			_glsl.cutOff = light->cutOff;
			_glsl.outerCutOff = light->outerCutOff;
			_glsl.diffuseStrength = light->diffuseStrength;
			_glsl.specularStrength = light->specularStrength;
			_glsl.attenuationConstant = light->attenuationConstant;
			_glsl.attenuationLinear = light->attenuationLinear;
			_glsl.attenuationQuadratic = light->attenuationQuadratic;
			_shadowMapGLSL.pcfSamples = light->shadowPCFSamples;
			_shadowMapGLSL.minBias = light->shadowMapMinBias;
			_shadowMapGLSL.maxBias = light->shadowMapMaxBias;

			glBindTexture(GL_TEXTURE_2D, light->shadowMap.texture);
			_shadowMapGLSL.lightSpaceMatrix = light->getLightSpaceMatrix();

			kreogl::shapes::drawSphere();
		}

		glCullFace(GL_BACK);
	}

	void SpotLightShader::updateShadowMap(const SpotLight & light, const DrawParams & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		const ScopedBindFramebuffer bind(light.shadowMap.frameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		const auto shaders = params.shaderPipeline.getShaders(ShaderStep::ShadowMap);
		if (!shaders)
			return;

		for (const auto shader : *shaders) {
			const auto shadowMapShader = static_cast<ShadowMapShader *>(shader);
			shadowMapShader->draw(light, params);
		}
	}
}