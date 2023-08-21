//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program, model_loc, view_loc, projection_loc;

mat4 model, model_ban;

GLfloat xx = 0, yy = 0, zz = 0, a = 0, zn = 0;
GLfloat l = -1.0, r = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 1.5, zFar = 5.0;


void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry( void )
{
	initCube();
	makeColorCube();
}


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW );

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors); 
	
}


void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	//
	model_loc = glGetUniformLocation(program, "Model");
	view_loc = glGetUniformLocation(program, "View");
	projection_loc = glGetUniformLocation(program, "projection");

	//
	glEnable(GL_DEPTH_TEST);

    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

void matban() {
	mat4 instance = Scale(1.2, 0.02, 0.6);

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void chanban(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * Scale(0.06, 0.8, 0.06);

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanban() {
	matban();
	chanban(-0.57, -0.41, 0.27);
	chanban(-0.57, -0.41, -0.27);
	chanban(0.57, -0.41, 0.27);
	chanban(0.57, -0.41, -0.27);
}

void ngankeo() {
	mat4 instance =  Scale(0.54, 0.02, 0.6);

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ban() {
	GLfloat aa = DegreesToRadians * a;

	model_ban = Translate(xx, yy, zz) * RotateY(aa);
	thanban();

	model_ban = model_ban * Translate(0, 0, zn) * Translate(-0.27, -0.15, 0);
	ngankeo();
}

void display( void )
{
	
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );                
    //glDrawArrays( GL_TRIANGLES, 0, NumPoints );    /*Vẽ các tam giác*/

	//
	/*point4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);*/

	vec4 eye = vec4(-1, 1, 3, 1);
	vec4 at = vec4(0, 0, 0, 0);
	vec4 up = vec4(0, 1, 0, 1);

	mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);

	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

	//model = RotateY(150);

	/*matban();
	thanban();
	ngankeo();*/
	ban();
	glutSwapBuffers();									   
}


void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
		case 033:			// 033 is Escape key octal value
			exit(1);		// quit program
			break;
		case 'x':
			xx = xx + 0.1;
			glutPostRedisplay();
			break;
		case 'X':
			xx = xx - 0.1;
			glutPostRedisplay();
			break;
		case 'y':
			yy = yy + 0.1;
			glutPostRedisplay();
			break;
		case 'Y':
			yy = yy - 0.1;
			glutPostRedisplay();
			break;
		case 'z':
			zz = zz + 0.1;
			glutPostRedisplay();
			break;
		case 'Z':
			zz = zz - 0.1;
			glutPostRedisplay();
			break;
		case 'a':
			a = a + 5;
			if (a > 360) a = 360;
			glutPostRedisplay();
			break;
		case 'A':
			a = a - 5;
			if (a < 0) a = 0;
			glutPostRedisplay();
			break;
		case 'N':
			zn = zn - 0.1;
			if (zn < 0) zn = 0;
			glutPostRedisplay();
			break;
		case 'n':
			zn = zn + 0.1;
			if (zn > 0.6) zn = 0.6;
			glutPostRedisplay();
			break;
    }
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );                  

	glutMainLoop();
    return 0;
}
