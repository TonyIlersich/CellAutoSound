#include "shaders.h"

const std::string melodyCode = R"___(

#version 130

uniform sampler2D texture;

const vec3 black = vec3(0.0, 0.0, 0.0);
const vec3 white = vec3(1.0, 1.0, 1.0);

vec3 old(int dx, int dy)
{
	vec2 step = vec2(dx, dy) / vec2(textureSize(texture, 0));
	vec2 target = gl_TexCoord[0].xy + step;
	vec2 wrapped = fract(vec2(1.0, 1.0) + target);
	return texture2D(texture, wrapped).rgb;
}

void main()
{
	/* do something cool */

	vec3 clr = old(0, 0);
	gl_FragColor = vec4(clr, 1.0);
}

)___";