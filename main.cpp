// #####################################################################
//  Main
//  Dartmouth COSC 77/177 Computer Graphics, Final Project
//  Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//  Modified by Georgia Dawahare, Amanda Sun, and Claire Draeger
// #####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Common.h"
#include "Driver.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#define PI = 3.1415926535

#endif

class FinalProjectDriver : public Driver, public OpenGLViewer
{
	using Base = Driver;
	std::vector<OpenGLTriangleMesh *> mesh_object_array; // mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
		draw_bk = false;   // turn off the default background and use the customized one
		draw_axes = false; // if you don't like the axes, turn them off!
		startTime = clock();
		OpenGLViewer::Initialize();
	}

	void Add_Shaders()
	{

		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("ocean.vert", "ocean.frag", "ocean");
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("noise1.vert", "noise1.frag", "noise1");
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("noise2.vert", "noise2.frag", "noise2");
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("noise3.vert", "noise3.frag", "noise3");
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("skybox.vert", "skybox.frag", "skybox");
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("dolphin.vert", "dolphin.frag", "dolphin");
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("marble.vert", "marble.frag", "marble");
	}

	void Add_Textures()
	{
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("sky.jpg", "skybox_albedo");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("dolphin_texture.jpg", "dolphin_albedo");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("ocean.jpeg", "ocean_albedo");
	}

	// Create skybox
	int Add_Skybox()
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		// add a sphere with radius=1. if the obj file name is not specified
		real radius = 15.;
		Initialize_Sphere_Mesh(radius, &mesh_obj->mesh, 3);

		// set up shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));

		// set up texture
		mesh_obj->Add_Texture("skybox_albedo", OpenGLTextureLibrary::Get_Texture("skybox_albedo"));
		Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj, ShadingMode::Texture);

		// initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	// Generate animated water
	int Add_Water()
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
		auto &mesh = mesh_obj->mesh;

		// vertex position
		std::vector<Vector3> triangle_vertices = {Vector3(-10, 0.5, -10), Vector3(10, 0.5, -10), Vector3(-10, 0.5, 10), Vector3(10, 0.5, 10)};
		std::vector<Vector3> &vertices = mesh_obj->mesh.Vertices();
		vertices = triangle_vertices;

		// vertex color
		std::vector<Vector4f> &vtx_color = mesh_obj->vtx_color;
		vtx_color = {Vector4f(1.f, 0.f, 0.f, 1.f), Vector4f(0.f, 1.f, 0.f, 1.f), Vector4f(0.f, 0.f, 1.f, 1.f), Vector4f(1.f, 1.f, 0.f, 1.f)};

		// vertex normal
		std::vector<Vector3> &vtx_normal = mesh_obj->vtx_normal;
		vtx_normal = {Vector3(0., 1., 0.), Vector3(0., 1., 0.), Vector3(0., 1., 0.), Vector3(0., 1., 0.)};

		// vertex uv
		std::vector<Vector2> &uv = mesh_obj->mesh.Uvs();
		uv = {Vector2(0., 0.), Vector2(1., 0.), Vector2(0., 1.), Vector2(1., 1.)};

		// mesh elements
		std::vector<Vector3i> &elements = mesh_obj->mesh.Elements();
		elements = {Vector3i(0, 1, 3), Vector3i(0, 3, 2)};

		// set up shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("ocean"));

		// set up texture
		mesh_obj->Add_Texture("ocean_albedo", OpenGLTextureLibrary::Get_Texture("ocean_albedo"));
		Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj, ShadingMode::Texture);

		// initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	// Add middle dolphin in the back
	int Add_Dolphin()
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		// read mesh file
		std::string obj_file_name = "dolphin.obj";
		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];

		// Loop over the values of vertices on the CPU end to translate dolphin to correct location
		std::vector<Vector3> &vertices = mesh_obj->mesh.Vertices();
		int vn = (int)vertices.size();
		for (int i = 0; i < vn; i++)
		{
			vertices[i] = Vector3(0.004 * vertices[i][0], 0.004 * vertices[i][1], 0.004 * vertices[i][2]);
			vertices[i] += Vector3(0.25, 1.5, -10);
		}

		// set up shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("dolphin"));

		// set up texture
		mesh_obj->Add_Texture("dolphin_albedo", OpenGLTextureLibrary::Get_Texture("dolphin_albedo"));
		Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj, ShadingMode::Texture);

		// initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	// Add rightmost dolphin
	int Add_Dolphin1()
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		// read mesh file
		std::string obj_file_name = "dolphin.obj";
		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];

		// Loop over the values of vertices on the CPU end to translate dolphin to correct location
		std::vector<Vector3> &vertices = mesh_obj->mesh.Vertices();
		int vn = (int)vertices.size();
		for (int i = 0; i < vn; i++)
		{

			vertices[i] = Vector3(0.006 * vertices[i][0], 0.006 * vertices[i][1], -0.006 * vertices[i][2]);
			vertices[i] += Vector3(-5, 1.5, 1);
		}

		// set up shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("dolphin"));

		// set up texture
		mesh_obj->Add_Texture("dolphin_albedo", OpenGLTextureLibrary::Get_Texture("dolphin_albedo"));
		Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj, ShadingMode::Texture);

		// initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	// Add leftmost dolphin
	int Add_Dolphin2()
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		// read mesh file
		std::string obj_file_name = "dolphin.obj";
		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];

		// Loop over the values of vertices on the CPU end to translate dolphin to correct location
		std::vector<Vector3> &vertices = mesh_obj->mesh.Vertices();
		int vn = (int)vertices.size();
		for (int i = 0; i < vn; i++)
		{
			vertices[i] = Vector3(0.007 * vertices[i][0], 0.007 * vertices[i][1], -0.007 * vertices[i][2]);
			vertices[i] += Vector3(5.25, 1.5, -5);
		}

		// set up shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("dolphin"));

		// set up texture
		mesh_obj->Add_Texture("dolphin_albedo", OpenGLTextureLibrary::Get_Texture("dolphin_albedo"));
		Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj, ShadingMode::Texture);

		// initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	// Add leftmost glacier
	int Add_Glaciers1()
	{
		int obj_idx = Add_Obj_Mesh_Object("plane.obj");
		auto plane_obj = mesh_object_array[obj_idx];

		std::vector<Vector3> &vertices = plane_obj->mesh.Vertices();
		int vn = (int)vertices.size();

		float theta = 90.0;
		glm::mat4 model = glm::mat4(1.0f);											  // create an identity matrix
		model = glm::rotate(model, glm::radians(theta), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around x-axis
		for (int i = 0; i < vn; i++)
		{
			glm::vec4 result = model * glm::vec4(vertices[i][0], vertices[i][1], vertices[1][2], 1.);
			vertices[i] = Vector3(1.25 * result.x, 1.25 * result.y, 1.25 * result.z);
			vertices[i] += Vector3(-3.5, 1., -5);
		}

		plane_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("noise2"));
		Set_Polygon_Mode(plane_obj, PolygonMode::Fill);
		Set_Shading_Mode(plane_obj, ShadingMode::Texture);

		plane_obj->Set_Data_Refreshed();
		plane_obj->Initialize();
	}

	// Add middle glaciers
	int Add_Glaciers2()
	{
		int obj_idx = Add_Obj_Mesh_Object("plane.obj");
		auto plane_obj = mesh_object_array[obj_idx];

		std::vector<Vector3> &vertices = plane_obj->mesh.Vertices();
		int vn = (int)vertices.size();

		float theta = 90.0;
		glm::mat4 model = glm::mat4(1.0f);											  // create an identity matrix
		model = glm::rotate(model, glm::radians(theta), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around x-axis
		for (int i = 0; i < vn; i++)
		{
			glm::vec4 result = model * glm::vec4(vertices[i][0], vertices[i][1], vertices[1][2], 1.);
			vertices[i] = Vector3(1.25 * result.x, 1.25 * result.y, 1.25 * result.z);
			vertices[i] += Vector3(-7, -3, -11);
		}

		plane_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("noise1"));
		Set_Polygon_Mode(plane_obj, PolygonMode::Fill);
		Set_Shading_Mode(plane_obj, ShadingMode::Texture);

		plane_obj->Set_Data_Refreshed();
		plane_obj->Initialize();
	}

	// Add leftmost set of glacier
	int Add_Glaciers3()
	{
		int obj_idx = Add_Obj_Mesh_Object("plane.obj");
		auto plane_obj = mesh_object_array[obj_idx];

		std::vector<Vector3> &vertices = plane_obj->mesh.Vertices();
		int vn = (int)vertices.size();
		float theta = 90.0;
		glm::mat4 model = glm::mat4(1.0f);											  // create an identity matrix
		model = glm::rotate(model, glm::radians(theta), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around x-axis
		for (int i = 0; i < vn; i++)
		{
			glm::vec4 result = model * glm::vec4(vertices[i][0], vertices[i][1], vertices[1][2], 1.);
			vertices[i] = Vector3(1.25 * result.x, 1.25 * result.y, 1.25 * result.z);
			vertices[i] += Vector3(3, 0., -13);
		}

		plane_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("noise3"));
		Set_Polygon_Mode(plane_obj, PolygonMode::Fill);
		Set_Shading_Mode(plane_obj, ShadingMode::Texture);

		plane_obj->Set_Data_Refreshed();
		plane_obj->Initialize();
	}

	// This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];
		std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	// Generate statue with pattern created with perlin noise
	int Add_Statue()
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		// read mesh file
		std::string obj_file_name = "statue.obj";
		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];

		std::vector<Vector3> &vertices = mesh_obj->mesh.Vertices();
		int vn = (int)vertices.size();
		for (int i = 0; i < vn; i++)
		{
			vertices[i] = Vector3(0.01 * vertices[i][0], 0.01 * vertices[i][1], -0.01 * vertices[i][2]);
			vertices[i] += Vector3(0, 4, 0);
		}

		// set up shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("marble"));

		// set up texture
		mesh_obj->Add_Texture("dolphin_albedo", OpenGLTextureLibrary::Get_Texture("dolphin_albedo"));
		Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj, ShadingMode::Texture);

		// initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	virtual void Initialize_Data()
	{
		Add_Shaders();
		Add_Textures();
		Add_Skybox();
		Add_Water();
		Add_Glaciers1();
		Add_Glaciers2();
		Add_Glaciers3();
		Add_Dolphin();
		Add_Dolphin1();
		Add_Dolphin2();
		// Add_Statue();	// TODO: Uncomment to see statue hovering face down over glaciers
	}

	// Go to next frame
	virtual void Toggle_Next_Frame()
	{
		for (auto &mesh_obj : mesh_object_array)
		{
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc, char *argv[])
{
	FinalProjectDriver driver;
	driver.Initialize();
	driver.Run();
}

#endif