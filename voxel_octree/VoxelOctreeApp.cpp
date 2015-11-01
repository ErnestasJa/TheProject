#include "Precomp.h"
#include "VoxelOctreeApp.h"
#include "core/CoreInc.h"
#include "motree/MotreeInc.h"
#include "opengl/OpenGLInc.h"
#include "utility/Logger.h"
#include "utility/Timer.h"
#include "renderer/Renderer.h"
#include "resources/ResourcesInc.h"
#include "application/SettingsManager.h"
#include "application/Window.h"
#include "player/Player.h"
#include <boost/algorithm/string/replace.hpp>


VoxelOctreeApp * VoxelOctreeApp::m_instance = nullptr;

VoxelOctreeApp * VoxelOctreeApp::Instance()
{
	return m_instance;
}

VoxelOctreeApp::VoxelOctreeApp(uint32_t argc, const char ** argv): Application(argc, argv)
{
	m_instance = this;
}

VoxelOctreeApp::~VoxelOctreeApp()
{

}

void VoxelOctreeApp::InitResources()
{
	auto loader = GetContext().GetResourceManager();
	sh = loader->LoadShader("shaders/solid_cube");
	cam = share(new Camera(glm::vec3(0, 0, -5), glm::vec3(0, 0, 5), glm::vec3(0, 1, 0), 1.7777777f, 45.0f, 0.1, 1024.0f));

	octree = share(new MortonOctTree());
	collisionManager = new CollisionManager(octree);
	octreeGen = new VoxMeshManager(octree);
	octreeGen->GenAllChunks();
	player = new Player(cam, collisionManager, glm::vec3(3, 3, 3));
	cube = new CubeMesh(player->GetAABB());
	bvoxLoader = share(new BVoxLoader(octree, GetContext().GetLogger()));
}

bool VoxelOctreeApp::Init()
{
	/*if(!InitSimple("Voxel octree application"))
		return false;*/

	if(!InitContextBasics() || !InitFileSystem() || !LoadConfig())
	{
		return false;
	}

	//If we want to override some settings for window.
	auto & video_group = GetContext().GetApplicationSettingsManager()->GetGroup("video");
	video_group.GetVar("window_width").Value(int(1280));
	video_group.GetVar("window_height").Value(int(720));
	InitWindowAndOpenGL("Supper awesome window, waow");

	GetContext().GetWindow()->SigKeyEvent().connect(sigc::mem_fun(this, &VoxelOctreeApp::OnKeyEvent));
	GetContext().GetWindow()->SigMouseKey().connect(sigc::mem_fun(this, &VoxelOctreeApp::OnMouseKey));
	GetContext().GetWindow()->SigMouseMoved().connect(sigc::mem_fun(this, &VoxelOctreeApp::OnMouseMove));

	renderer = share(new Renderer(nullptr));
	renderer->Enable({GL_DEPTH_TEST, GL_CULL_FACE});
	renderer->SetCullFace(GL_BACK);
	renderer->SetFrontFace(GL_CCW);
	renderer->SetClearColor(0.4, 0.8, 0.2, 0.0);
	InitResources();
	AfterInit();
	GetContext().GetTimer()->tick();
	return true;
}

void VoxelOctreeApp::ClearOctree()
{
	octree->GetChildNodes().clear();
	octreeGen->GetMeshes().clear();
}

void VoxelOctreeApp::GenerateOctreeMeshes()
{
	octree->SortLeafNodes();
	octree->RemoveDuplicateNodes();
	octreeGen->GenAllChunks();
}

bool VoxelOctreeApp::LoadLevel(const std::string & levelName)
{
	ClearOctree();
	bvoxLoader->ReadFile(levelName);
	GenerateOctreeMeshes();
}

bool VoxelOctreeApp::SaveLevel(const std::string & levelName)
{
	bvoxLoader->WriteFile(levelName);
}

void VoxelOctreeApp::AfterInit()
{
}

void VoxelOctreeApp::SetPlayerPosition(float x, float y, float z)
{
	player->GetPosition().x = x;
	player->GetPosition().y = y;
	player->GetPosition().z = z;

	player->GetVelocity().x = 0;
	player->GetVelocity().y = 0;
	player->GetVelocity().z = 0;
}

static bool renderWireframe = false;
static float speed = 0.0;
bool wk = false, ak = false, sk = false, dk = false;
bool VoxelOctreeApp::Update()
{
	if (GetContext().GetWindow()->Update() && !GetContext().GetWindow()->GetShouldClose() && !GetContext().GetWindow()->GetKey(GLFW_KEY_ESCAPE))
	{
		GetContext().GetTimer()->tick();

		///PLAYER MOVE CODE
		auto look = cam->GetLook();
		auto right = cam->GetRight();
		look.y = 0;
		right.y = 0;

		look = glm::normalize(look) * speed;
		right = glm::normalize(right) * speed;

		if (wk)
		{
			player->GetVelocity().x = look.x;
			player->GetVelocity().z = look.z;
		}
		else if (sk)
		{
			player->GetVelocity().x = -look.x;
			player->GetVelocity().z = -look.z;
		}

		if (dk)
		{
			player->GetVelocity().x = right.x;
			player->GetVelocity().z = right.z;
		}
		else if (ak)
		{
			player->GetVelocity().x = -right.x;
			player->GetVelocity().z = -right.z;
		}

		if ((!(wk || ak || sk || dk)) && player->OnGround())
		{
			player->GetVelocity().x = 0;
			player->GetVelocity().z = 0;
		}
		///~PLAYER MOVE CODE END

		player->Update(((float)GetContext().GetTimer()->get_delta_time()) / 1000.0f);
		cam->Update(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 Model = glm::translate(glm::mat4(1.0f), player->GetPosition()); // glm::mat4(1.0f);//
		glm::mat4 MVP   = cam->GetViewProjMat() * Model;
		sh->GetBinding("mvp").Set(MVP);
		sh->Set();
		cube->Render(true, false);

		Model = glm::mat4(1.0f);
		MVP   = cam->GetViewProjMat() * Model;
		sh->GetBinding("mvp").Set(MVP);
		sh->Set();

		if (renderWireframe)
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		octreeGen->RenderAllMeshes();

		GetContext().GetOpenGLExtensionLoader()->check_and_output_errors();
		GetContext().GetWindow()->SwapBuffers();
		return true;
	}
	return false;
}

bool VoxelOctreeApp::Exit()
{
	return true;
}

void VoxelOctreeApp::OnWindowClose()
{

}

#define UPDATE_KEY(var,key_enum)\
  if(action == GLFW_PRESS && key==key_enum) var = true;\
  if(action == GLFW_RELEASE && key==key_enum) var = false


void VoxelOctreeApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT)
		speed = 20.0f;

	if (action == GLFW_RELEASE && key == GLFW_KEY_LEFT_SHIFT)
		speed = 5.0f;

	if (action == GLFW_PRESS && key == GLFW_KEY_LEFT_CONTROL)
		speed = 50.0f;

	if (action == GLFW_RELEASE && key == GLFW_KEY_LEFT_CONTROL)
		speed = 5.0f;

	if (action == GLFW_RELEASE && key == GLFW_KEY_U)
		SaveLevel("test_save.bvox");


	if (key == GLFW_KEY_SPACE)
	{
		player->Jump(20);
	}

	UPDATE_KEY(wk, GLFW_KEY_W);
	UPDATE_KEY(ak, GLFW_KEY_A);
	UPDATE_KEY(sk, GLFW_KEY_S);
	UPDATE_KEY(dk, GLFW_KEY_D);

	if (key == GLFW_KEY_1)
		renderWireframe = true;
	else if (key == GLFW_KEY_2)
		renderWireframe = false;
}

void VoxelOctreeApp::OnMouseMove(double x, double y)
{

}

void VoxelOctreeApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{
	glm::vec3 lookat = glm::normalize(cam->GetLook());
	glm::vec3 position = cam->GetPosition() + lookat; // shoot one unit in front of player
	GetContext().GetLogger()->log(LOG_LOG, "cam pos: [%f, %f, %f]", position.x, position.y, position.z);

	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			CollisionInfo info(position, lookat);
			collisionManager->Collide(info, 0, glm::ivec3(0));

			if (info.nearestDistance < INFINITY)
			{
				uint32_t x, y, z;
				decodeMK(info.node.start, x, y, z);
				GetContext().GetLogger()->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]", x, y, z);

				auto it = std::lower_bound(octree->GetChildNodes().begin(), octree->GetChildNodes().end(), info.node);
				decodeMK(it->start, x, y, z);
				GetContext().GetLogger()->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]", x, y, z);

				uint32_t mk = info.node.start & CHUNK_MASK;
				if (it != octree->GetChildNodes().end())
				{
					octree->GetChildNodes().erase(it);
					octreeGen->RebuildChunk(mk);
				}
			}
			break;
		}
		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			CollisionInfo info(position, lookat);
			collisionManager->Collide(info, 0, glm::ivec3(0));

			if (info.nearestDistance < INFINITY)
			{
				uint32_t x, y, z;
				decodeMK(info.node.start, x, y, z);
				GetContext().GetLogger()->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]", x, y, z);

				/// add node
				VoxelSide side = collisionManager->GetCollisionSide(glm::vec3(x, y, z), position, lookat);
				glm::ivec3 pos = VoxelSideToPosition(side);

				if ((pos.x == -1 && x == 0) || (pos.y == -1 && y == 0) || (pos.z == -1 && z == 0))
					break;

				x += pos.x;
				y += pos.y;
				z += pos.z;

				uint32_t mk = encodeMK(x, y, z);

				octree->AddNode(MNode(mk, 1, 128, 128 , 128));
				octreeGen->RebuildChunk(mk & CHUNK_MASK);
			}
			break;
		}
		}
	}
}

std::string VoxelOctreeApp::GetApplicationId()
{
	return "voxel_octree";
}

MortonOctTreePtr VoxelOctreeApp::GetOctree()
{
	return octree;
}