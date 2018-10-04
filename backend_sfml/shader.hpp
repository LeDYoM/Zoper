#pragma once

#ifndef LIB_BACKEND_SFML_SHADER_INCLUDE_HPP
#define LIB_BACKEND_SFML_SHADER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <lib/include/backend/ishader.hpp>

#include <SFML/Graphics/Shader.hpp>

namespace lib::backend::sfmlb
{
	class Shader : public IShader
	{
	public:
        Shader(uptr<sf::Shader> shader);
        ~Shader() override;

        void setUniform(const str &name, vector2df v) override;
        void setUniform(const str &name, ITexture *texture) override;

	private:
        uptr<sf::Shader> m_shaderPrivate;
	};
}

#endif