#include "../libraries/Libraries.h"

using namespace std;

float timePrev;
float h = 0.1f;
float t = 0.0f;
float dt = 0.0f;
Eigen::Vector3f g(0.0f, 0.0f, 0.0f);
Eigen::Vector2f mouse;
Eigen::Vector2f center;
bool keyDown[256] = {false};

Program prog;
Camera camera;
Scene scene;

bool cull = false;
bool line = false;

// Model matrix for the plane
Eigen::Matrix4f T;
Eigen::Matrix3f T1 = Eigen::Matrix3f::Identity();
Eigen::Vector3f lightPosCam;

void loadScene()
{
	//lightPosCam = (0.0f, 0.0f, 0.0f);
	// time
	timePrev = 0.0f;
	t = 0.0f;

	//tex_diffuse.setFilename("materials/diffuse.bmp");

	// Populate the world here
	scene.load();

	prog.setShaderNames("../source/simple_vert.glsl", "../source/simple_frag.glsl");
}

void initGL()
{
	//////////////////////////////////////////////////////
	// Initialize GL for the whole scene
	//////////////////////////////////////////////////////
	
	// Set background color
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	//////////////////////////////////////////////////////
	// Initialize the geometry
	//////////////////////////////////////////////////////

	scene.init();

	//////////////////////////////////////////////////////
	// Intialize the shaders
	//////////////////////////////////////////////////////
	
	prog.init();
	prog.addUniform("P");
	prog.addUniform("MV");
	prog.addUniform("T1");
	prog.addAttribute("vertPos");
	prog.addAttribute("vertNor");
	prog.addAttribute("vertTex");
	prog.addUniform("lightPos");
	prog.addUniform("intensity");
	prog.addUniform("ka");
	prog.addUniform("kd");
	prog.addUniform("ks");
	prog.addUniform("s");
	prog.addUniform("texture");


	// Initialize textures
	//tex_diffuse.init();

	// Check GLSL
	GLSL::checkVersion();
}

void reshapeGL(int w, int h)
{
	// Set view size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// Set center of screen
	center(0) = (float)w / 2;
	center(1) = (float)h / 2;
	// Set camera aspect ratio
	camera.setWindowSize(w, h);
	glutWarpPointer((int)center(0), (int)center(1));
}

void drawGL()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable backface culling
	if(cull) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(line) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// display statistics
	char str[50];
	sprintf(str, "FPS: %0.2f   # Objects: %d   Score %d", (1.0f / dt), scene.getNumUncollectedObjects(), scene.getPlayer().getNumCollided());
	HUD::drawString(0, (center(1) * 2.0f - 12.0f) / center(1) / 2.0f, str);

	// Create matrix stacks
	MatrixStack P, MV;
	// Apply camera transforms
	P.pushMatrix();
	camera.applyProjectionMatrix(&P);
	MV.pushMatrix();
	camera.applyViewMatrix(&MV);

	// Get light position in Camera space
	Eigen::Vector4f lightPos = MV.topMatrix() * Eigen::Vector4f(-500.0f, 200.0f, -500.0f, 1.0f);

	// Bind the program
	prog.bind();

	// Send shader info to the GPU
	glUniform3fv(prog.getUniform("lightPos"), 1, lightPos.data());
	glUniform1f(prog.getUniform("intensity"), 1.0f);
	glUniform3fv(prog.getUniform("ka"),  1, Eigen::Vector3f(0.3f, 0.3f, 0.3f).data());
	glUniform3fv(prog.getUniform("kd"),  1, Eigen::Vector3f(0.8f, 0.7f, 0.7f).data());
	glUniform3fv(prog.getUniform("ks"), 1, Eigen::Vector3f(1.0f, 0.9f, 0.8f).data());
	glUniform1f(prog.getUniform("s"), 200.0f);

	/////
	// Draw shapes
	glUniformMatrix4fv(prog.getUniform("P"), 1, GL_FALSE, P.topMatrix().data());
	scene.draw(MV, &prog);

	// Unbind
	glUseProgram(0);

	// Pop stacks
	MV.popMatrix();
	P.popMatrix();

	// Double buffer
	glutSwapBuffers();
}


void passiveMotionGL(int x, int y)
{
	mouse(0) = x;
	mouse(1) = y;
}

void keyboardGL(unsigned char key, int x, int y)
{
	keyDown[key] = true;
	switch(key) {
		case 27:
			// ESCAPE
			exit(0);
			break;
		case 'c':
			cull = !cull;
			break;
		case 'l':
			line = !line;
			break;
      case ' ':
         break;
	}
}

void keyboardUpGL(unsigned char key, int x, int y)
{
	keyDown[key] = false;
}

void timerGL(int value)
{
	float timeCurr = (float)glutGet(GLUT_ELAPSED_TIME)/1000;
	dt = timeCurr - timePrev;
	timePrev = timeCurr;
	scene.update(keyDown, mouse, center, dt);
	Player player = scene.getPlayer();
	camera.update(player.getYaw(), player.getPitch(), player.getPosition());
	glutWarpPointer((int)center(0), (int)center(1));
	glutPostRedisplay();
	glutTimerFunc(20, timerGL, 0);
}

void processArgOptions(int argc, char **argv) {
	// default fullscreen
	bool fullscreen = true;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-debug") == 0)
			fullscreen = false;
		else {
			std::cout << "Unknown command line argument: " << argv[i] << std::endl;
			exit(-1);
		}
	}

	if (fullscreen)
		glutFullScreen();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	center = Eigen::Vector2f(300.0f, 300.0f);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("Noah Harper, Kyle Lozier");

	processArgOptions(argc, argv);

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer((int)center(0), (int)center(1));
	glutPassiveMotionFunc(passiveMotionGL);
	glutKeyboardFunc(keyboardGL);
	glutKeyboardUpFunc(keyboardUpGL);
	glutReshapeFunc(reshapeGL);
	glutDisplayFunc(drawGL);
	glutTimerFunc(20, timerGL, 0);
	loadScene();
	initGL();
	glutMainLoop();
	return 0;
}
