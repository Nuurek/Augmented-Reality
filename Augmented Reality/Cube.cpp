/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "cube.h"

namespace Models {
	
	Cube cube;
	
	Cube::Cube() {
		vertices=CubeInternal::vertices;
		normals=CubeInternal::normals;
		vertexNormals=CubeInternal::vertexNormals;
		texCoords=CubeInternal::texCoords;
		colors=CubeInternal::colors;
		vertexCount=CubeInternal::vertexCount;
	}
	
	Cube::~Cube() {
	}
	
	void Cube::drawSolid() {
		glEnable(GL_NORMALIZE);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(4,GL_FLOAT,0,vertices);
		//glColorPointer(4,GL_FLOAT,0,colors);
		glNormalPointer(GL_FLOAT,sizeof(float)*4,vertexNormals);
		glTexCoordPointer(2,GL_FLOAT,0,texCoords);
		
		glDrawArrays(GL_TRIANGLES,0,vertexCount);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	
	namespace CubeInternal {
			unsigned int vertexCount=36;

			float vertices[]={
				1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				
				
				-1.0f,-1.0f, 1.0f,1.0f,	
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f, 1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				
			}; 

			float colors[]={
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
			};

			float normals[]={
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
				
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
			};
			
			float vertexNormals[]={
				1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				
				
				-1.0f,-1.0f, 1.0f,1.0f,	
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f, 1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
			};

			float texCoords[]={
				0.000059f, 1.0f-0.000004f,
			0.000103f, 1.0f-0.336048f,
			0.335973f, 1.0f-0.335903f,
			1.000023f, 1.0f-0.000013f,
			0.667979f, 1.0f-0.335851f,
			0.999958f, 1.0f-0.336064f,
			0.667979f, 1.0f-0.335851f,
			0.336024f, 1.0f-0.671877f,
			0.667969f, 1.0f-0.671889f,
			1.000023f, 1.0f-0.000013f,
			0.668104f, 1.0f-0.000013f,
			0.667979f, 1.0f-0.335851f,
			0.000059f, 1.0f-0.000004f,
			0.335973f, 1.0f-0.335903f,
			0.336098f, 1.0f-0.000071f,
			0.667979f, 1.0f-0.335851f,
			0.335973f, 1.0f-0.335903f,
			0.336024f, 1.0f-0.671877f,
			1.000004f, 1.0f-0.671847f,
			0.999958f, 1.0f-0.336064f,
			0.667979f, 1.0f-0.335851f,
			0.668104f, 1.0f-0.000013f,
			0.335973f, 1.0f-0.335903f,
			0.667979f, 1.0f-0.335851f,
			0.335973f, 1.0f-0.335903f,
			0.668104f, 1.0f-0.000013f,
			0.336098f, 1.0f-0.000071f,
			0.000103f, 1.0f-0.336048f,
			0.000004f, 1.0f-0.671870f,
			0.336024f, 1.0f-0.671877f,
			0.000103f, 1.0f-0.336048f,
			0.336024f, 1.0f-0.671877f,
			0.335973f, 1.0f-0.335903f,
			0.667969f, 1.0f-0.671889f,
			1.000004f, 1.0f-0.671847f,
			0.667979f, 1.0f-0.335851f
			};
	}
}