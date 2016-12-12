#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


in vec2 texCoord0;
out vec2 iTexCoord0; 

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color;  //kolor wierzcholka (na razie ignorowany)
in vec4 normal; //wektor normalny w wierzcholku (na razie ignorowany)
out vec4 iColor;

void main(void) {
	iColor = color;
	gl_Position=P*V*M*vertex;
	iTexCoord0=texCoord0;
}

