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

	OBJloader* objLoader = new OBJloader();
	objLoader->loadOBJ("data/mustang.obj");
	glModels.push_back(objLoader->getGlModel(shaderProgram, "data/mustang.png"));
	delete objLoader;

	objLoader = new OBJloader();
	objLoader->loadOBJ("data/football.obj");
	glModels.push_back(objLoader->getGlModel(shaderProgram, "data/football.png"));
	delete objLoader;

	objLoader = new OBJloader();
	objLoader->loadOBJ("data/dog.obj");
	glModels.push_back(objLoader->getGlModel(shaderProgram, "data/dog.png"));
	delete objLoader;

	objLoader = new OBJloader();
	objLoader->loadOBJ("data/cube.obj");
	glModels.push_back(objLoader->getGlModel(shaderProgram, "data/testTex.png"));
	delete objLoader;
	

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
void Drawer::drawScene(cv::Mat *frame, std::vector<glm::mat4> cameraMatrix, std::vector<int> objectId) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 P = glm::perspective(glm::radians(39.f), 4.f / 3.f, 0.1f, 100.0f);

	glm::mat4 M = glm::mat4(1.0f);

	readFrame(frame, currentFrameTex);
	for (int i = 0; i < cameraMatrix.size(); i++) {
		if (objectId[i] >= glModels.size()) objectId[i] = 0;
		drawObject(glModels[objectId[i]]->vao, shaderProgram, P, cameraMatrix[i], M, glModels[objectId[i]]->vertexCount, glModels[objectId[i]]->texture);
	}

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
	for (auto model = glModels.begin(); model != glModels.end(); model++) {
		glDeleteVertexArrays(1, &((*model)->vao));
	}
	glDeleteVertexArrays(1, &vao); //Usuniêcie vao
	glDeleteVertexArrays(1, &backgroundVAO);


}
