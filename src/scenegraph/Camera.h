#ifndef SG_CAMERA_OBJECT_H
#define SG_CAMERA_OBJECT_H

struct AppContext;

class Camera
{
public:
    Camera(AppContext* ctx, const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio=1.777777f, float field_of_view=45.0f, float near_z=1.0f, float far_z=4096.0f);
    virtual ~Camera();

    virtual glm::mat4 & GetProjectionMat();

	const glm::vec3 GetLook() const;
	const glm::vec3 GetPosition() const;

	const float GetFar() const;
	const float GetNear() const;
	const float GetFOV() const;
	const float GetAspectRatio() const;

    virtual void Update(float dt);

public:
	void Walk(const float amount);
	void Strafe(const float amount);
	void Lift(const float amount);
	virtual glm::mat4x4 GetViewMat();
	virtual glm::mat4x4 GetViewProjMat();
	void Orbit(glm::vec3 point,float distance,float angleX,float angleY);
private:
    void HandleMouse();
protected:
    float m_fov, m_aspect_ratio, m_far, m_near;
	static glm::vec3 UP;
	glm::vec3 m_pos;
	glm::vec3 m_look;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::mat4 m_P; //projection matrix
	glm::quat m_rot;

	glm::ivec2 m_last_mouse_pos,m_current_mouse_pos;

	glm::vec3 m_translation;

	bool m_fps;
	AppContext* _appContext;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif // SG_CAMERA_OBJECT_H
