#include "Drawer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lodepng.h"
#include <stdlib.h>
#include <stdio.h>

Drawer::Drawer()
{
}


Drawer::~Drawer()
{
	freeOpenGLProgram();
}

void Drawer::init(int frameWidth, int frameHeight)
{
	GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno

	//glfwSetErrorCallback(error_callback); //Zarejestruj procedur� obs�ugi b��d�w

	if (!glfwInit()) { //Zainicjuj bibliotek� GLFW
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}
	aspectRatio = (float)frameHeight / (float)frameWidth;
	window = glfwCreateWindow(frameWidth, frameHeight, "AR Markers", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
	{
		fprintf(stderr, "Nie mo�na utworzy� okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
	glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotek� GLEW
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW.\n");
		exit(EXIT_FAILURE);
	}
	this->window = window;
	initOpenGLProgram(window); //Operacje inicjuj�ce
}
//Procedura obs�ugi b��d�w
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
void Drawer::initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
	glClearColor(0, 0, 1, 1); //Czy�� ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W��cz u�ywanie Z-Bufora


	shaderProgram = new ShaderProgram("vshader.vert", NULL, "fshader.frag"); //Wczytaj program cieniuj�cy 


																		   //*****Przygotowanie do rysowania pojedynczego obiektu*******
																		   //Zbuduj VBO z danymi obiektu do narysowania
	bufVertices = makeBuffer(model.vertices, model.vertexCount, sizeof(float) * 4); //VBO ze wsp�rz�dnymi wierzcho�k�w
	bufColors = makeBuffer(model.colors, model.vertexCount, sizeof(float) * 4);//VBO z kolorami wierzcho�k�w
	bufNormals = makeBuffer(model.normals, model.vertexCount, sizeof(float) * 4);//VBO z wektorami normalnymi wierzcho�k�w
	bufTexCoords = makeBuffer(model.texCoords, model.vertexCount, sizeof(float) * 2);
																	 //Zbuduj VAO wi���cy atrybuty z konkretnymi VBO
	glGenVertexArrays(1, &vao); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

	glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(shaderProgram, "vertex", bufVertices, 4); //"vertex" odnosi si� do deklaracji "in vec4 vertex;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "color", bufColors, 4); //"color" odnosi si� do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "normal", bufNormals, 4); //"normal" odnosi si� do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "texCoord0", bufTexCoords, 2);

	glBindVertexArray(0); //Dezaktywuj VAO
						  //******Koniec przygotowania obiektu************

	tex0 = readTexture("testTex.png");

}
void Drawer::assignVBOtoAttribute(ShaderProgram *shaderProgram, char* attributeName, GLuint bufVBO, int vertexSize) {
	GLuint location = shaderProgram->getAttribLocation(attributeName); //Pobierz numery slot�w dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //W��cz u�ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj� by� brane z aktywnego VBO
}
//Tworzy bufor VBO z tablicy
GLuint Drawer::makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), kt�ry b�dzie zawiera� tablic� danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablic� do VBO

	return handle;
}
//Procedura rysuj�ca zawarto�� sceny
void Drawer::drawScene(cv::Mat frame, float angle) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolor�w

	glm::mat4 P = glm::perspective(50 * PI / 180, 4.f/3.f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(0.0f, 0.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));


	//Wylicz macierz modelu rysowanego obiektu
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle, glm::vec3(0, 1, 0));
	//M = glm::rotate(M, angle_y, glm::vec3(0, 1, 0));

	//tex0 = frame.data;
	currentFrameTex = readFrame(frame);

	//Narysuj obiekt
	drawObject(vao, shaderProgram, P, V, M);
	//model.drawSolid();

	//Przerzu� tylny bufor na przedni
	glfwSwapBuffers(window);

}
void Drawer::drawObject(GLuint vao, ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {
	//W��czenie programu cieniuj�cego, kt�ry ma zosta� u�yty do rysowania
	//W tym programie wystarczy�oby wywo�a� to raz, w setupShaders, ale chodzi o pokazanie, 
	//�e mozna zmienia� program cieniuj�cy podczas rysowania jednej sceny
	shaderProgram->use();

	//Przeka� do shadera macierze P,V i M.
	//W linijkach poni�ej, polecenie:
	//  shaderProgram->getUniformLocation("P") 
	//pobiera numer slotu odpowiadaj�cego zmiennej jednorodnej o podanej nazwie
	//UWAGA! "P" w powy�szym poleceniu odpowiada deklaracji "uniform mat4 P;" w vertex shaderze, 
	//a mP w glm::value_ptr(mP) odpowiada argumentowi  "mat4 mP;" TYM pliku.
	//Ca�a poni�sza linijka przekazuje do zmiennej jednorodnej P w vertex shaderze dane z argumentu mP niniejszej funkcji
	//Pozosta�e polecenia dzia�aj� podobnie.
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(mM));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, currentFrameTex);
	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi�za� slot�w atrybut�w z tablicami z danymi
	glBindVertexArray(vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);

	//Posprz�tanie po sobie (niekonieczne w sumie je�eli korzystamy z VAO dla ka�dego rysowanego obiektu)
	glBindVertexArray(0);
}
GLFWwindow* Drawer::getWindow() {
	return window;
}
GLuint Drawer::readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pami�ci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
GLuint Drawer::readFrame(cv::Mat frame) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	if (!frame.isContinuous()) {
		std::cout << "Error: Frame is not continuous!!!\n";
		return tex0;
	}
	cv::resize(frame, frame, cv::Size(512, 512));

	unsigned width = frame.cols, height = frame.rows;
	std::vector<uchar> image(frame.rows*frame.cols);
	image.assign(frame.datastart, frame.dataend);
	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0,
		GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
void Drawer::freeOpenGLProgram() {
	delete shaderProgram; //Usuni�cie programu cieniuj�cego

	glDeleteVertexArrays(1, &vao); //Usuni�cie vao
	glDeleteBuffers(1, &bufVertices); //Usuni�cie VBO z wierzcho�kami
	glDeleteBuffers(1, &bufColors); //Usuni�cie VBO z kolorami
	glDeleteBuffers(1, &bufNormals); //Usuni�cie VBO z wektorami normalnymi
	glDeleteBuffers(1, &bufTexCoords); //Usuni�cie VBO ze wsp�rz�dnymi teksturowania


}
