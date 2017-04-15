#include "iostream.h"
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement(float );

Camera camera(Vec3(0,255,0), Vec3(0, 0, 0), Vec3(0, 1, 0));
Windows gWindow("Learn", 800, 600);
GameTimer gTimer;
mat4 Projection;
Frustum frustum;
Font font;
Input gInput;
// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.1" << std::endl;
	gWindow.InitWindow();
	gTimer.Init();
	Resources::Init();
	font.Init(800.0, 600.0);
	Vec2 size = gWindow.GetWindowSize();
	frustum.Init(45.0f, (float)size.x / (float)size.y, 1.0f, 5000.0f);
	frustum.Init();
	Shader simple("Game\\Shader\\Simple.vs", "Game\\Shader\\Simple.frag");
	simple.LinkShader();
	Shader SunShader("Game\\Shader\\Sun.vs", "Game\\Shader\\Sun.frag");
	SunShader.LinkShader();
	Shader Screen("Game\\Shader\\Quad.vs", "Game\\Shader\\Quad.frag");
	Screen.LinkShader();
	
	Texture* contaner = Resources::LoadTexture("Game\\Texture\\awesomeface.png");
	Texture* contaner2 = Resources::LoadTexture("Game\\Texture\\container2.png");
	Texture* contaner2_spec = Resources::LoadTexture("Game\\Texture\\container2_specular.png");
	Texture* matrix_tex = Resources::LoadTexture("Game\\Texture\\matrix.jpg");
	Quad quad(2,300);
	Cube Sun(0.5f);
	Cube cube;
	Axis AXIS(100);
	mat4 View = Math::LookAt(Vec3(100, 100, 100), Vec3(0, 0, 0), Vec3(0, 1, 0));
	
	//LTBRenderer* pLTB = new LTBRenderer();;
	//pLTB->Init("Game\\Model\\NANO_TERMINATOR\\M-MOTION-TERMINATOR.LTB");
	
	LTBRenderer* pLTB2 = new LTBRenderer();;
	pLTB2->Init("Game\\Model\\CHARACTER\\C707_BODY_BL.LTB");
	pLTB2->SetAnimation(3);
	
	Projection = Math::Perspective(45.0f, (float)size.x / (float)size.y, 1.0f, 2000.0f);
	
	
	
	
	PointLight pointLight1(Vec3(0, 255, 0), Vec3(0.8f, 0.8f, 0.8f), Vec3(0.9f, 0.9f, 0.9f), Vec3(0.2f, 0.2f, 0.2f));
	pointLight1.SetAttenuation(1.0f, 0.0014f, 0.000007f);
	PointLight pointLight3(Vec3(2000, 255, 0), Vec3(0.8f, 0.8f, 0.8f), Vec3(0.1f, 0.2f, 0.1f), Vec3(0.2f, 0.2f, 0.2f));
	pointLight3.SetAttenuation(1.0f, 0.0014f, 0.000007f);
	PointLight pointLight4(Vec3(0, 255, 2000), Vec3(0.8f, 0.8f, 0.8f), Vec3(0.1f, 0.2f, 0.1f), Vec3(0.2f, 0.2f, 0.2f));
	pointLight4.SetAttenuation(1.0f, 0.0014f, 0.000007f);
	PointLight pointLight2(Vec3(10, 10, -5), Vec3(0.2f, 0.2f, 0.2f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
	pointLight2.SetAttenuation(1.0f, 0.0014f, 0.000007f);
	DirectionLight dirLight(Vec3(5), Vec3(0.1f), Vec3(0.5f), Vec3(1.0f));
	
	Spotlight spotLight(Vec3(0), Math::Cos(Math::ToRadian(15.5f)), Math::Cos(Math::ToRadian(17.5f)), pointLight2);

	//MapRenderer Quake;
	//Quake.Init("Game\\maps\\Level.bsp");
	
	
	//RenderTarget PostProcessing((int)size.x, (int)size.y);
	//MSSA* mssa = new MSSA((int)size.x, (int)size.y, 4);
	//mssa->SetDrawFBO(PostProcessing.FBO());
	
	SkyBox* pSky = new SkyBox;
	
	pSky->Init();
	


	Scene* pScene = new Scene;
	Terrain* pTerrain = new Terrain;
	pTerrain->Init("coastMountain513.raw", 1.5f, 0.0f);
	pSky->AddChild(pTerrain);
	pScene->SetRootScene(pSky);
	camera.SetSpeed(300);
	pScene->SetCamera(&camera);
	pScene->SetFrustum(&frustum);
	//pScene->AddPointLight(pointLight1);
	//pScene->AddPointLight(pointLight3);
	//pScene->AddPointLight(pointLight4);
	//pScene->AddSpotLight(spotLight);
	string text = "OpenGL 3.1";

	
	gTimer.Reset();
	// Game loop
	while (!glfwWindowShouldClose(gWindow.Window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		gTimer.Tick();

		//if (gInput.Press(GLFW_KEY_N)) pLTB->NextAnim();
		if (gInput.Press(GLFW_KEY_M)) pLTB2->NextAnim();

		mat4 mtras;
		mtras.Translate(200, 120, 0);
		//pLTB->SetTransform(mtras);
		//pLTB->Update(gTimer.GetDeltaTime());
		pLTB2->Update(gTimer.GetDeltaTime());
		pScene->Update(gTimer.GetDeltaTime());
		//frustum.Update(camera);
		//camera.Update(gTimer.GetDeltaTime());
		//PostProcessing.BeginFrame();
		//mssa->BeginFrame();
	//	mat4 Model;
	//	mat4 View = camera.GetViewMatrix();
	//	mat4 SS = View;
	//	SS.Translate(0, 0, 0);
		
		// Render sky box

		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	
		pScene->RenderScene();
		//glDepthFunc(GL_LESS);

		
		//glDisable(GL_CULL_FACE);
		

	
		//glEnable(GL_CULL_FACE);
		
		/*
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
		mat4 Model;
		for (int i = -5; i < 5; i++)
		{
			Model.Translate(0.0, 0.0, i * 5.0f + 10.0f);
			//if (!frustum.Inside(Vec3(0, 0, i * 5.0f + 10.0f))) continue;
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
		
		
		
		/*SunShader.Use();
		mat4 SunModel;
		//SunModel.Translate(5, 5, 5);
		//SunShader.SetUniformMatrix("Model", SunModel.ToFloatPtr());
		SunShader.SetUniformMatrix("View", View.ToFloatPtr());
		SunShader.SetUniformMatrix("Proj", Projection.ToFloatPtr());
		SunShader.SetUniformMatrix("Model", mat4().ToFloatPtr());
		glBindVertexArray(quad.m_iVAO);
		glDrawElements(quad.Topology, quad.m_vIndices.size(), GL_UNSIGNED_INT, 0);
		*/
		/*glBindVertexArray(AXIS.m_iVAO); 
		SunShader.SetUniformMatrix("Model", mat4().ToFloatPtr());
		glDrawElements(AXIS.Topology, AXIS.m_vIndices.size(), GL_UNSIGNED_INT, 0);
		*/
		
		
		//terrain.PreRender(View, Projection, camera);
		//terrain.Render();
		//terrain.PostRender();
		
		
		//pLTB->PreRender(camera.GetViewMatrix(), frustum.GetProjMatrix(),camera);
		//pLTB->Render();
		//pLTB->PostRender();


		pLTB2->PreRender(camera.GetViewMatrix(), frustum.GetProjMatrix(), camera);
		pLTB2->Render();
		pLTB2->PostRender();
		

		//glDepthFunc(GL_LEQUAL);
		//sky.Render(SS, Projection);
		
		/*
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
		*/

		glDisable(GL_CULL_FACE);
		std::stringstream ss;
		ss << gTimer.GetFPS();
		text = string("FPS: ") + ss.str();
		font.Draw(text, 0, 580);


		glfwSwapBuffers(gWindow.Window());
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	
	Resources::Release();
	Log::OutputFile();
	delete pScene;
	//delete pLTB;
	delete pLTB2;
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
/*
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key < 0) return;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		gWindow.SwitchMode();
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		bUpdateF = !bUpdateF;

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}
*/
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

	float dx = (float)(xpos - last_x);
	float dy = -(float)(ypos - last_y);
	last_x = xpos;
	last_y = ypos;
	camera.OnMouse(dx, dy);
}
void scroll_callback(GLFWwindow*, double x, double y)
{
	//cout << x << y << endl;
	//mNear += y;
	//Projection = Math::Perspective(30.0f, 4.0 / 3.0, mNear, 500.0f);
	//frustum.Init(30.0f, 4.0f / 3.0f, mNear, 500.0f);
}