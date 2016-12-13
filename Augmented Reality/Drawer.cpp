#include "Drawer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lodepng.h"
#include <stdlib.h>
#include <stdio.h>
#include "OBJloader.h"

Drawer::Drawer()
{
}


Drawer::~Drawer()
{
	freeOpenGLProgram();
}

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
	
}
void Drawer::init(int frameWidth, int frameHeight)
{
	GLFWwindow* window = nullptr; //WskaŸnik na obiekt reprezentuj¹cy okno

	glfwSetErrorCallback(error_callback); //Zarejestruj procedurê obs³ugi b³êdów
	glfwSetWindowSizeCallback(window, window_size_callback);

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}
	aspectRatio = (float)frameHeight / (float)frameWidth;
	window = glfwCreateWindow(frameWidth, frameHeight, "AR Markers", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		exit(EXIT_FAILURE);
	}
	this->window = window;
	initOpenGLProgram(window); //Operacje inicjuj¹ce
}
void Drawer::initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 1, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_POINT_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);


	GLuint bufVertices; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych wierzcho³ków
	GLuint bufColors;  //Uchwyt na bufor VBO przechowuj¹cy tablicê kolorów
	GLuint bufNormals; //Uchwyt na bufor VBO przechowuj¹cy tablickê wektorów normalnych
	GLuint bufTexCoords;

	shaderProgram = new ShaderProgram("vshader.vert", NULL, "fshader.frag"); //Wczytaj program cieniuj¹cy
	backgroundShaderProgram = new ShaderProgram("backgroundVshader.vert", NULL, "backgroundFshader.frag");	

	OBJloader cubeLoader;
	cubeLoader.loadOBJ("data/mustang.obj");
	glModels.push_back(cubeLoader.getGlModel(shaderProgram, "data/mustang.png"));
	
		//model
	bufVertices = makeBuffer(model.vertices, model.vertexCount, sizeof(float) * 4); //VBO ze wspó³rzêdnymi wierzcho³ków
	bufColors = makeBuffer(model.colors, model.vertexCount, sizeof(float) * 4);//VBO z kolorami wierzcho³ków
	bufNormals = makeBuffer(model.normals, model.vertexCount, sizeof(float) * 4);//VBO z wektorami normalnymi wierzcho³ków
	bufTexCoords = makeBuffer(model.texCoords, model.vertexCount, sizeof(float) * 2);
																	 //Zbuduj VAO wi¹¿¹cy atrybuty z konkretnymi VBO
	glGenVertexArrays(1, &vao); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

	glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(shaderProgram, "vertex", bufVertices, 4); //"vertex" odnosi siê do deklaracji "in vec4 vertex;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "color", bufColors, 4); //"color" odnosi siê do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "normal", bufNormals, 4); //"normal" odnosi siê do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "texCoord0", bufTexCoords, 2);

	glBindVertexArray(0); 
	glDeleteBuffers(1, &bufVertices); //Usuniêcie VBO z wierzcho³kami
	glDeleteBuffers(1, &bufColors); //Usuniêcie VBO z kolorami
	glDeleteBuffers(1, &bufNormals); //Usuniêcie VBO z wektorami normalnymi
	glDeleteBuffers(1, &bufTexCoords); //Usuniêcie VBO ze wspó³rzêdnymi teksturowania

	//Background
	bufVertices = makeBuffer(backgroundModel.vertices, backgroundModel.vertexCount, sizeof(float) * 4); //VBO ze wspó³rzêdnymi wierzcho³ków
	bufColors = makeBuffer(backgroundModel.colors, backgroundModel.vertexCount, sizeof(float) * 4);//VBO z kolorami wierzcho³ków
	bufNormals = makeBuffer(backgroundModel.normals, backgroundModel.vertexCount, sizeof(float) * 4);//VBO z wektorami normalnymi wierzcho³ków
	bufTexCoords = makeBuffer(backgroundModel.texCoords, backgroundModel.vertexCount, sizeof(float) * 2);

	glGenVertexArrays(1, &backgroundVAO); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
	


	glBindVertexArray(backgroundVAO); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(backgroundShaderProgram, "vertex", bufVertices, 4); //"vertex" odnosi siê do deklaracji "in vec4 vertex;" w vertex shaderze
	assignVBOtoAttribute(backgroundShaderProgram, "color", bufColors, 4); //"color" odnosi siê do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute(backgroundShaderProgram, "normal", bufNormals, 4); //"normal" odnosi siê do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute(backgroundShaderProgram, "texCoord0", bufTexCoords, 2);

	glBindVertexArray(0);


	glDeleteBuffers(1, &bufVertices); //Usuniêcie VBO z wierzcho³kami
	glDeleteBuffers(1, &bufColors); //Usuniêcie VBO z kolorami
	glDeleteBuffers(1, &bufNormals); //Usuniêcie VBO z wektorami normalnymi
	glDeleteBuffers(1, &bufTexCoords); //Usuniêcie VBO ze wspó³rzêdnymi teksturowania

	tex0 = readTexture("testTex.png");
	currentFrameTex = initFrameTexture();

}
void Drawer::assignVBOtoAttribute(ShaderProgram *shaderProgram, char* attributeName, GLuint bufVBO, int vertexSize) {
	GLuint location = shaderProgram->getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}
//Tworzy bufor VBO z tablicy
GLuint Drawer::makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}
//Procedura rysuj¹ca zawartoœæ sceny
void Drawer::drawScene(cv::Mat *frame, std::vector<glm::mat4> cameraMatrix) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glm::mat4 P = glm::perspective(glm::radians(50.f), 4.f/3.f, 0.1f, 100.0f);

	glm::mat4 M = glm::mat4(1.0f);

	readFrame(frame, currentFrameTex);
	for(auto cam=cameraMatrix.begin();cam!=cameraMatrix.end();cam++)
		drawObject(glModels[0]->vao, shaderProgram, P, *cam, M, glModels[0]->vertexCount, glModels[0]->texture);

	drawBackground(backgroundVAO, backgroundShaderProgram, backgroundModel, currentFrameTex);

	glfwSwapBuffers(window);

}
void Drawer::drawObject(GLuint vao, ShaderProgram *shader, mat4 mP, mat4 mV, mat4 mM, int vertexCount, GLuint texture) {
	shader->use();

	glUniformMatrix4fv(shader->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shader->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shader->getUniformLocation("M"), 1, false, glm::value_ptr(mM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi¹zañ slotów atrybutów z tablicami z danymi
	glBindVertexArray(vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	//Posprz¹tanie po sobie (niekonieczne w sumie je¿eli korzystamy z VAO dla ka¿dego rysowanego obiektu)
	glBindVertexArray(0);
}
void Drawer::drawBackground(GLuint vao, ShaderProgram *shader, Models::Model object, GLuint texture) {
	shader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi¹zañ slotów atrybutów z tablicami z danymi
	glBindVertexArray(vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);

	//Posprz¹tanie po sobie (niekonieczne w sumie je¿eli korzystamy z VAO dla ka¿dego rysowanego obiektu)
	glBindVertexArray(0);
}
GLFWwindow* Drawer::getWindow() {
	return window;
}
GLuint Drawer::readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
GLuint Drawer::initFrameTexture() {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt

	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
void Drawer::readFrame(cv::Mat *frame, GLuint tex) {
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame->cols, frame->rows, 0,
	GL_BGR, GL_UNSIGNED_BYTE, frame->ptr());
}
void Drawer::freeOpenGLProgram() {
	delete shaderProgram; //Usuniêcie programu cieniuj¹cego
	delete backgroundShaderProgram;

	glDeleteVertexArrays(1, &vao); //Usuniêcie vao
	glDeleteVertexArrays(1, &backgroundVAO);


}
