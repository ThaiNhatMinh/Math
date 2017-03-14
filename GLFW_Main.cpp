#include "iostream.h"
#include "Camera.h"
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement(float );

Camera camera(Vec3(10, 10, 20), Vec3(0, 0, 0), Vec3(0, 1, 0));
Windows gWindow("Learn", 800, 600);
GameTimer gTimer;
bool keys[1024];


// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.1" << std::endl;
	gWindow.InitWindow();
	gTimer.Init();
	Vec2 size = gWindow.GetWindowSize();
	Shader simple("Game\\Shader\\Simple.vs", "Game\\Shader\\Simple.frag");
	Shader SunShader("Game\\Shader\\Sun.vs", "Game\\Shader\\Sun.frag");
	Shader Screen("Game\\Shader\\Quad.vs", "Game\\Shader\\Quad.frag");
	
	Font font;
	font.Init(size.x, size.y);
	Texture* contaner = Resources::LoadTexture("Game\\Texture\\awesomeface.png");
	Texture* contaner2 = Resources::LoadTexture("Game\\Texture\\container2.png");
	Texture* contaner2_spec = Resources::LoadTexture("Game\\Texture\\container2_specular.png");
	Texture* matrix_tex = Resources::LoadTexture("Game\\Texture\\matrix.jpg");
	Quad quad;
	Cube cube;
	Cube Sun(0.5f);
	mat4 View = Math::LookAt(Vec3(100, 100, 100), Vec3(0, 0, 0), Vec3(0, 1, 0));
	mat4 Projection = Math::Perspective(30.0f, (float)size.x / (float)size.y, 1.0f, 500.0f);
	camera.SetSpeed(25.0f);
	gTimer.Reset();
	
	PointLight pointLight1(Vec3(5, 5, 5), Vec3(0.2f, 0.2f, 0.2f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
	pointLight1.SetAttenuation(1.0f, 0.09f, 0.0075f);
	PointLight pointLight2(Vec3(10, 10, -5), Vec3(0.2f, 0.2f, 0.2f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
	pointLight2.SetAttenuation(1.0f, 0.09f, 0.0075f);
	DirectionLight dirLight(Vec3(5), Vec3(0.1f), Vec3(0.5f), Vec3(1.0f));
	
	Spotlight spotLight(Vec3(0), Math::Cos(Math::ToRadian(12.5f)), Math::Cos(Math::ToRadian(17.5f)), pointLight1);


	RenderTarget PostProcessing(size.x, size.y);
	MSSA* mssa = new MSSA(size.x, size.y, 4);
	mssa->SetDrawFBO(PostProcessing.FBO());
	SkyBox sky;
	vector<string> filelist;
	filelist.push_back("Game\\Texture\\skybox\\right.jpg");
	filelist.push_back("Game\\Texture\\skybox\\left.jpg");
	filelist.push_back("Game\\Texture\\skybox\\top.jpg");
	filelist.push_back("Game\\Texture\\skybox\\bottom.jpg");
	filelist.push_back("Game\\Texture\\skybox\\back.jpg");
	filelist.push_back("Game\\Texture\\skybox\\front.jpg");
	sky.Init();
	sky.LoadCubeTexture(filelist);
	string text = "OpenGL 3.1";

	MapRenderer Quake;
	Quake.Init("Game\\maps\\Level.bsp");
	Shader Map("Game\\Shader\\basic_shader.vert", "Game\\Shader\\basic_shader.frag");
	camera.SetSpeed(100);
	// Game loop
	while (!glfwWindowShouldClose(gWindow.Window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		gTimer.Tick();

		Do_Movement(gTimer.GetDeltaTime());
		
		//PostProcessing.BeginFrame();
		mssa->BeginFrame();
		mat4 Model;
		mat4 View = camera.GetViewMatrix();
		mat4 SS = View;
		SS.Translate(0, 0, 0);
		// Render sky box

		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sky.Render(SS, Projection);
		//glDepthFunc(GL_LESS);

		


		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		simple.Use();

		simple.SetUniform("EyePos", camera.GetPosition());
		simple.SetUniformMatrix("View", View.ToFloatPtr());
		simple.SetUniformMatrix("Proj", Projection.ToFloatPtr());
		simple.SetUniform("material.shininess", 128.0f);
		spotLight.Position = camera.GetPosition();
		spotLight.Direction = camera.GetFront();

		spotLight.SendData(&simple);
		dirLight.SendData(&simple);
		pointLight1.SendData(&simple, 0);
		pointLight2.SendData(&simple, 1);



		contaner2->Bind(0);
		simple.SetUniform("diffuseMap", 0);
		contaner2_spec->Bind(1);
		simple.SetUniform("specularMap", 1);

		glBindVertexArray(cube.m_iVAO);

		for (int i = -5; i < 5; i++)
		{
			Model.Translate(0, 0, i * 5 + 10);
			simple.SetUniformMatrix("Model", Model.ToFloatPtr());
			glDrawArrays(cube.Topology, 0, cube.m_vPositions.size());
		}

		SunShader.Use();
		mat4 SunModel;
		SunModel.Translate(5, 5, 5);
		SunShader.SetUniformMatrix("Model", SunModel.ToFloatPtr());
		SunShader.SetUniformMatrix("View", View.ToFloatPtr());
		SunShader.SetUniformMatrix("Proj", Projection.ToFloatPtr());
		glBindVertexArray(Sun.m_iVAO);
		glDrawArrays(Sun.Topology, 0, Sun.m_vPositions.size());
		SunModel.Translate(10, 10, -5);
		SunShader.SetUniformMatrix("Model", SunModel.ToFloatPtr());
		glDrawArrays(Sun.Topology, 0, Sun.m_vPositions.size());
		
		

		/* Render BSP data */
		Quake.PreRender(View, Projection);
		Quake.Render();
		Quake.PostRender();
	



		
		mssa->EndFrame();
		
		PostProcessing.EndFrame();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		Screen.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, PostProcessing.TextureID());
		Screen.SetUniform("renderTexture", 0);
		glBindVertexArray(quad.m_iVAO);
		glDrawElements(quad.Topology, quad.m_vIndices.size(), GL_UNSIGNED_INT, 0);
		
		std::stringstream ss;
		ss << gTimer.GetFPS();
		text = string("FPS: ") + ss.str();
		font.Draw(text, 0, 580);


		glfwSwapBuffers(gWindow.Window());
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	
	Resources::Release();
	Log::OutputFile();
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

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		gWindow.SwitchMode();

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
