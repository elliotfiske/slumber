#include "../libraries/Libraries.h"

using namespace std;

float timePrev;
float h = 0.1f;
float t = 0.0f;
float dt = 0.0f;
int width, height;
Eigen::Vector3f g(0.0f, 0.0f, 0.0f);
Eigen::Vector2f mouse;
Eigen::Vector2f center;
bool keyDown[256] = {false};
bool keyToggles[256] = { false };

Program prog_pass1;
Program prog_pass2;
Light light;
Camera camera;
Scene scene;

GLuint shadowmap_width = 2048;
GLuint shadowmap_height = 2048;

bool cull = false;
bool line = false;

// Model matrix for the plane
Eigen::Matrix4f T;
Eigen::Matrix3f T1 = Eigen::Matrix3f::Identity();
Eigen::Vector3f lightPosCam;

GLuint framebufferID;
GLuint shadowMap;

void loadScene()
{
	//lightPosCam = (0.0f, 0.0f, 0.0f);
	// time
	timePrev = 0.0f;
	t = 0.0f;

	//tex_diffuse.setFilename("materials/diffuse.bmp");

	// Populate the world here
	scene.load();

	prog_pass1.setShaderNames("../source/Pass1_vert.glsl", "../source/Pass1_frag.glsl");
	prog_pass2.setShaderNames("../source/Pass2_vert.glsl", "../source/Pass2_frag.glsl");
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
	
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowmap_width, shadowmap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cerr << "Framebuffer is not ok" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//////////////////////////////////////////////////////
	// Initialize the geometry
	//////////////////////////////////////////////////////

	scene.init();

	//////////////////////////////////////////////////////
	// Intialize the shaders
	//////////////////////////////////////////////////////
	
	prog_pass1.init();
	prog_pass1.addUniform("MVP");
	prog_pass1.addAttribute("vertPos");

	prog_pass2.init();
	prog_pass2.addUniform("P");
	prog_pass2.addUniform("MV");
	prog_pass2.addUniform("lightMVP");
	prog_pass2.addUniform("T1");
	prog_pass2.addAttribute("vertPos");
	prog_pass2.addAttribute("vertNor");
	prog_pass2.addAttribute("vertTex");
	prog_pass2.addUniform("shadowMap");
	prog_pass2.addUniform("lightPos");
	prog_pass2.addUniform("ka");
	prog_pass2.addUniform("kd");
	prog_pass2.addUniform("ks");
	prog_pass2.addUniform("texture");


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
	width = w;
	height = h;
	// Set camera aspect ratio
	camera.setWindowSize(w, h);
	glutWarpPointer((int)center(0), (int)center(1));
}

void drawGL()
{
	// Clear buffers
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	}*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// display statistics
	char str[50];
	sprintf(str, "FPS: %0.2f   # Objects: %d   Score %d", (1.0f / dt), scene.getNumUncollectedObjects(), scene.getPlayer().getNumCollided());
	HUD::drawString(0, (height - 12.0f) / height, str);

	// Create matrix stacks
	MatrixStack P, MV;
	// Apply camera transforms
	P.pushMatrix();
	camera.applyProjectionMatrix(&P);
	MV.pushMatrix();
	camera.applyViewMatrix(&MV);

	//light.setPosition(Eigen::Vector3f(0.0f, 5.0f, 0.0f));

	// Get light position in Camera space
	Eigen::Vector4f lightPos = MV.topMatrix() * Eigen::Vector4f(light.getPosition()(0), light.getPosition()(1), light.getPosition()(2), 1.0f);


	// Pass 1: get depth from light source
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glViewport(0, 0, shadowmap_width, shadowmap_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	prog_pass1.bind();

	scene.draw(MV, &prog_pass1, light, true);

	prog_pass1.unbind();

	// Pass 2: draw with shadow info
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	prog_pass2.bind();
	glUniform3fv(prog_pass2.getUniform("lightPos"), 1, lightPos.data());
	glUniform3fv(prog_pass2.getUniform("ka"),  1, Eigen::Vector3f(0.3f, 0.3f, 0.3f).data());
	glUniform3fv(prog_pass2.getUniform("kd"),  1, Eigen::Vector3f(0.8f, 0.7f, 0.7f).data());
	glUniform3fv(prog_pass2.getUniform("ks"), 1, Eigen::Vector3f(1.0f, 0.9f, 0.8f).data());
	glUniformMatrix4fv(prog_pass2.getUniform("P"), 1, GL_FALSE, P.topMatrix().data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(prog_pass2.getUniform("shadowMap"), 0);

	scene.draw(MV, &prog_pass2, light, false);

	prog_pass2.unbind();

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
	keyToggles[key] = !keyToggles[key];
	keyDown[key] = true;
	Eigen::Vector3f lightPos = light.getPosition();
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
		case 'x':
			lightPos(0) += 0.1;
			light.setPosition(lightPos);
			break;
		case 'X':
			lightPos(0) -= 0.1;
			light.setPosition(lightPos);
			break;
		case 'y':
			lightPos(1) += 0.1;
			light.setPosition(lightPos);
			std::cout << lightPos(1) << std::endl;
			break;
		case 'Y':
			lightPos(1) -= 0.1;
			light.setPosition(lightPos);
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
	if (keyDown['p'])
		dt = 0.0f;
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
	width = height = 600;
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
