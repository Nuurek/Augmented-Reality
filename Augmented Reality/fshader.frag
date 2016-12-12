#version 330

uniform sampler2D textureMap0;

in vec2 iTexCoord0;
out vec4 pixelColor;

in vec4 iColor;
void main(void) {
	pixelColor = texture(textureMap0,iTexCoord0);
	//pixelColor = vec4(1,1,1,0);
	pixelColor = iColor;
}