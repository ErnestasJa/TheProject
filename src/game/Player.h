#ifndef PLAYER_H
#define PLAYER_H

#include "opengl/Camera.h"
#include "motree/CollisionManager.h"
#include "opengl/AABB.h"

static glm::vec3 gGravity = glm::vec3(0,1,0);

class Player
{
 public:
  Player(CameraPtr cam, CollisionManager * octree, glm::vec3 position, AABB aabb = AABB(glm::vec3(0,0,0),glm::vec3(0.7,1,0.7)), glm::vec3 eyeOffset = glm::vec3(0,0.5,0));
  virtual ~Player();

  const AABB & GetAABB();
  glm::vec3 & GetVelocity();
  glm::vec3 & GetPosition();
  glm::vec3 & GetEyeOffset();
  CameraPtr GetCamera();

  void Update(float timeStep);
  void ApplyGravity();
  bool Jump(float velocity);
  bool OnGround();

  void SetFlyEnabled(bool enabled);
  bool GetFlyEnabled();
	

 protected:

  bool IsColliding();
  bool IsOnGround();
  bool IsSweptColliding(float timeStep);
  
  CollisionManager * m_octree;
  CameraPtr m_cam;
  glm::vec3   m_eyeOffset,
    m_position,
    m_velocity;
  AABB m_aabb;
  bool m_onGround, m_flyEnabled;
};

#endif // PLAYER_H
