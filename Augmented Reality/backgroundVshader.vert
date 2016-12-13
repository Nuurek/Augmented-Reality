#vesion 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec2 texCoord0;
out vec2 iTexCoord0;

in vec4 vertex;
in vec4 color;
in vec4 normal;
void main(void){
	gl_Position = vec4(vertex.xyz, 1.f);
	iTexCoord0 = texCoord0;
}