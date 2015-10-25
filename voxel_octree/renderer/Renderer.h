#ifndef RENDERER_H
#define RENDERER_H

#include "utility/Vector.h"
#include "AppForwardDecl.h"

typedef uint32_t GLEnum;
class Renderer
{
public:
	Renderer(MaterialPtr defaultMaterial);
	virtual ~Renderer();
	
	bool Enable(GLEnum glCapability);
	bool Enable(vector<GLEnum> glCapabilities);
	bool Disable(GLEnum glCapability);
	bool Disable(vector<GLEnum> glCapabilities);

	bool SetMaterial(MaterialPtr material);
	MaterialPtr GetMaterial();
	MaterialPtr GetDefaultMaterial();

	bool SetCamera(CameraPtr camera);
	CameraPtr GetCamera();

	void SetClearColor(glm::vec4 color);
	void SetClearColor(float r, float g, float b, float a);
	void SetCullFace(GLEnum cullFace);
	void SetFrontFace(GLEnum direction);

private:
	MaterialPtr m_activeMaterial;
	MaterialPtr m_defaultMaterial;
	CameraPtr   m_activeCamera;
};

#endif