/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "shaderprogram.h"
#include <string>
#include <fstream>
#include <sstream> 
//Procedura wczytuje plik do tablicy znaków.
char* ShaderProgram::readFile(char* fileName) {
	int filesize;
	FILE *plik;
	char* result;
	printf("loading %s\n", fileName);
	plik=fopen(fileName,"rb");
	fseek(plik,0,SEEK_END);
	filesize=ftell(plik);
	fseek(plik,0,SEEK_SET);
	result=new char[filesize+1];
	fread(result,1,filesize,plik);
	result[filesize]='\0';
	printf("size: %d", filesize);
	fclose(plik);

	return result;
}
std::string read_shader_file(const char *shader_file)
{
	// no feedback is provided for stream errors / exceptions.

	std::ifstream file(shader_file);
	if (!file) return std::string();

	file.ignore(std::numeric_limits<std::streamsize>::max());
	auto size = file.gcount();

	if (size > 0x10000) // 64KiB sanity check for shaders:
		return std::string();

	file.clear();
	file.seekg(0, std::ios_base::beg);

	std::stringstream sstr;
	sstr << file.rdbuf();
	file.close();

	return sstr.str();
}
//Metoda wczytuje i kompiluje shader, a następnie zwraca jego uchwyt
GLuint ShaderProgram::loadShader(GLenum shaderType, char* fileName) {
	//Wygeneruj uchwyt na shader
	GLuint shader=glCreateShader(shaderType);//shaderType to GL_VERTEX_SHADER, GL_GEOMETRY_SHADER lub GL_FRAGMENT_SHADER
	//Wczytaj plik ze źródłem shadera do tablicy znaków
	const GLchar* shaderSource=readFile(fileName);
	//Powiąż źródło z uchwytem shadera
	//std::string shaderStr = read_shader_file(fileName);
	//const char *c_str = shaderStr.c_str();
	//shaderSource = c_str;
	glShaderSource(shader,1,&shaderSource,NULL);
	//Skompiluj źródło
	glCompileShader(shader);
	//Usuń źródło shadera z pamięci (nie będzie już potrzebne)
	delete []shaderSource;
	
	//Pobierz log błędów kompilacji i wyświetl
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}
	
	//Zwróć uchwyt wygenerowanego shadera
	return shader;
}

ShaderProgram::ShaderProgram(char* vertexShaderFile,char* geometryShaderFile,char* fragmentShaderFile) {
	//Wczytaj vertex shader
	printf("Loading vertex shader...\n");
	vertexShader=loadShader(GL_VERTEX_SHADER,vertexShaderFile);

	//Wczytaj geometry shader
	if (geometryShaderFile!=NULL) {
		printf("Loading geometry shader...\n");
		geometryShader=loadShader(GL_GEOMETRY_SHADER,geometryShaderFile);
	} else {
		geometryShader=0;
	}

	//Wczytaj fragment shader
	printf("Loading fragment shader...\n");
	fragmentShader=loadShader(GL_FRAGMENT_SHADER,fragmentShaderFile);

	//Wygeneruj uchwyt programu cieniującego
	shaderProgram=glCreateProgram();

	//Podłącz do niego shadery i zlinkuj program
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	if (geometryShaderFile!=NULL) glAttachShader(shaderProgram,geometryShader);
	glLinkProgram(shaderProgram);

	//Pobierz log błędów linkowania i wyświetl
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}

	printf("Shader program created \n");
}

ShaderProgram::~ShaderProgram() {
	//Odłącz shadery od programu
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader!=0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	//Wykasuj shadery
	glDeleteShader(vertexShader);
	if (geometryShader!=0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//Wykasuj program
	glDeleteProgram(shaderProgram);
}


//Włącz używanie programu cieniującego reprezentowanego przez aktualny obiekt
void ShaderProgram::use() {
	glUseProgram(shaderProgram);
}

//Pobierz numer slotu odpowiadającego zmiennej jednorodnej o nazwie variableName
GLuint ShaderProgram::getUniformLocation(char* variableName) {
	return glGetUniformLocation(shaderProgram,variableName);
}

//Pobierz numer slotu odpowiadającego atrybutowi o nazwie variableName
GLuint ShaderProgram::getAttribLocation(char* variableName) {
	return glGetAttribLocation(shaderProgram,variableName);
}