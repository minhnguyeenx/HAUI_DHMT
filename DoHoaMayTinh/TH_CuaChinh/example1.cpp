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

mat4 model, model_cua, model_cua_2;

GLfloat xx = 0, yy = 0, zz = 0, cua1_a = 0, cua2_a = 0;

GLfloat l = -1, r = 1;
GLfloat bottom = -1, top = 1;
GLfloat zNear = 1, zFar = 5;


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

	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	projection_loc = glGetUniformLocation(program, "projection");

	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

void canh1() {
	mat4 instance = Translate(-0.2, 0, 0)*Scale(0.4, 1.2, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canh2() {
	mat4 instance = Translate(0.2, 0, 0)*Scale(0.4, 1.2, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cua_2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhcua() {
	mat4 instance = Translate(-0.4, 0, 0)*Scale(0.06, 1.32, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.43, 0, 0) * Scale(0.06, 1.32, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 0.63, 0) * Scale(0.8, 0.06, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, -0.63, 0) * Scale(0.8, 0.06, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void cua() {
	model_cua = Translate(xx, yy, zz);
	thanhcua();

	model_cua_2 = model_cua;

	model_cua = model_cua * Translate(-0.4, 0, 0) * RotateY(cua1_a) * Translate(0.4, 0, 0);
	canh1();

	model_cua_2 = model_cua_2 * Translate(0.4, 0, 0) * RotateY(cua2_a) * Translate(-0.4, 0, 0);
	canh2();
}


void display( void )
{
	
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );   

	vec4 eye = vec4(1, 1, 1, 1);
	vec4 at = vec4(0, 0, 0, 1);
	vec4 up = vec4(0, 1, 0, 1);

	mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);

	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);


	model = RotateY(180);
	/*thanhcua();
	canh1();
	canh2();*/
	cua();
    //glDrawArrays( GL_TRIANGLES, 0, NumPoints );    /*Vẽ các tam giác*/
	glutSwapBuffers();									   
}


void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
    case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;
	case 'l':
		xx = xx - 0.12;
		glutPostRedisplay();
		break;
	case 'r':
		xx = xx + 0.12;
		glutPostRedisplay();
		break;
	case 'm':
		cua1_a = cua1_a - 5;
		if (cua1_a < -180) cua1_a = -180;
		glutPostRedisplay();
		break;
	case 'M':
		cua1_a = cua1_a + 5;
		if (cua1_a > 0) cua1_a = 0;
		glutPostRedisplay();
		break;
	case 'N':
		cua2_a = cua2_a - 5;
		if (cua2_a < 0) cua2_a = 0;
		glutPostRedisplay();
		break;
	case 'n':
		cua2_a = cua2_a + 5;
		if (cua2_a > 180) cua2_a = 180;
		glutPostRedisplay();
		break;
    }
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 840, 840 );                 
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
