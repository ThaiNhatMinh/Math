#include "iostream.h"
#include "SOIL.h"
#include "Quad.h"
#include "Cube.h"
#include "Axis.h"
#include "Camera.h"
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement(float );
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera(Vec3(10, 10, 20), Vec3(0, 0, 0), Vec3(0, 1, 0));

GameTimer g_Timer;
bool keys[1024];


// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.1" << std::endl;
	// Init GLFW
	glfwInit();
	g_Timer.Init();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Create a GLFWwindow object that we can use for GLFW's functions

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	
	Shader simple("Simple.vs", "Simple.frag");
	Shader SunShader("Sun.vs", "Sun.frag");
	//Shader simple2D("Simple2D.vs", "Simple.frag");
	cl_texture_t* contaner = Resources::LoadTexture("awesomeface.png");
	cl_texture_t* contaner2 = Resources::LoadTexture("container2.png");
	cl_texture_t* contaner2_spec = Resources::LoadTexture("container2_specular.png");
	cl_texture_t* matrix_tex = Resources::LoadTexture("matrix.jpg");
	Quad quad;
	Cube cube;
	Cube Sun(0.5f);
	mat4 View = Math::LookAt(Vec3(100, 100, 100), Vec3(0, 0, 0), Vec3(0, 1, 0));
	mat4 Projection = Math::Perspective(30.0f, (float)WIDTH / (float)HEIGHT, 1.0f, 500.0f);
	camera.SetSpeed(25.0f);
	g_Timer.Reset();
	
	PointLight pointLight1(Vec3(5, 5, 5), Vec3(0.2f, 0.2f, 0.2f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
	pointLight1.SetAttenuation(1.0f, 0.09f, 0.0075f);
	PointLight pointLight2(Vec3(10, 10, -5), Vec3(0.2f, 0.2f, 0.2f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
	pointLight2.SetAttenuation(1.0f, 0.09f, 0.0075f);
	DirectionLight dirLight(Vec3(5), Vec3(0.1f), Vec3(0.5f), Vec3(1.0f));

	Spotlight spotLight(Vec3(0), Math::Cos(Math::ToRadian(12.5f)), Math::Cos(Math::ToRadian(17.5f)), pointLight1);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		g_Timer.Tick();

		Do_Movement(g_Timer.GetDeltaTime());
	
		// Render
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



		simple.Use();
		
		mat4 Model;
		mat4 View = camera.GetViewMatrix();

		
		simple.SetUniform("EyePos", camera.GetPosition());
		
		simple.SetUniformMatrix("View", View.ToFloatPtr());
		simple.SetUniformMatrix("Proj", Projection.ToFloatPtr());

		simple.SetUniform("material.shininess", 128.0f);


		spotLight.Position = camera.GetPosition();
		spotLight.Direction = camera.GetFront();
	
		spotLight.SendData(&simple);
		dirLight.SendData(&simple);
		pointLight1.SendData(&simple,0);
		pointLight2.SendData(&simple,1);
		
		
		
		contaner2->Bind(0);
		simple.SetUniform("diffuseMap", 0);
		contaner2_spec->Bind(1);
		simple.SetUniform("specularMap", 1);

		glBindVertexArray(cube.m_iVAO);

		for (int i = -5; i < 5; i++)
		{
			Model.Translate(0, 0, i*5 + 10);
			simple.SetUniformMatrix("Model", Model.ToFloatPtr());
			glDrawArrays(cube.Topology, 0, cube.m_vPositions.size());
		}
		//glDrawElements(cube.Topology, cube.m_vIndices.size(), GL_UNSIGNED_INT, 0);
		
		



	
		//mat4 View = camera.GetViewMatrix();
		SunShader.Use();
		mat4 SunModel;
		SunModel.Translate(5, 5, 5);
		SunShader.SetUniformMatrix("Model", SunModel.ToFloatPtr());
		SunShader.SetUniformMatrix("View", View.ToFloatPtr());
		SunShader.SetUniformMatrix("Proj", Projection.ToFloatPtr());
		glBindVertexArray(Sun.m_iVAO);
		//glDrawElements(Sun.Topology, Sun.m_vIndices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(Sun.Topology, 0, Sun.m_vPositions.size());
		SunModel.Translate(10, 10, -5);
		SunShader.SetUniformMatrix("Model", SunModel.ToFloatPtr());
		glDrawArrays(Sun.Topology, 0, Sun.m_vPositions.size());
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void Do_Movement(float deltaTime)
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.OnKeyboard(GLFW_KEY_W, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.OnKeyboard(GLFW_KEY_S, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.OnKeyboard(GLFW_KEY_A, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.OnKeyboard(GLFW_KEY_D, deltaTime);
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key < 0) return;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = false;
	static double last_x = 0;
	static double last_y = 0;
	
	if (!firstMouse)
	{
		last_x = xpos;
		last_y = ypos;
		firstMouse = true;
	}

	float dx = xpos - last_x;
	float dy = -(ypos - last_y);
	last_x = xpos;
	last_y = ypos;
	camera.OnMouse(dx, dy);
}