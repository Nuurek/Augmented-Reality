#version 330

uniform sampler2D textureMap0;

in vec2 iTexCoord0;
out vec4 pixelColor;

void main() {
	pixelColor = texture(textureMap0,iTexCoord0);
}