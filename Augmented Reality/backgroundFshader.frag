#version 330

uniform sampler2D textureMap1;


in vec2 iTexCoord0;
out vec4 pixelColor;

void main(void) {
	pixelColor = texture(textureMap1,iTexCoord0);
	//pixelColor = vec4(1,1,1,0);
}