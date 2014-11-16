#include "Precomp.h"
#include "Player.h"

Player::Player(CameraPtr cam, MortonOctTree<10>* octree, glm::vec3 position, AABB aabb, glm::vec3 eyeOffset)
{
    m_octree = octree;
    m_cam = cam;
    m_eyeOffset = eyeOffset;
    m_position = position;
    m_aabb = aabb;
    m_onGround = false;
}

Player::~Player()
{
    //dtor
}

glm::vec3 & Player::GetVelocity()
{
    return m_velocity;
}

glm::vec3 & Player::GetPosition()
{
    return m_position;
}

glm::vec3 & Player::GetEyeOffset()
{
    return m_eyeOffset;
}

CameraPtr Player::GetCamera()
{
    return m_cam;
}

AABB & Player::GetAABB()
{
    return m_aabb;
}


void Player::Update(float timeStep)
{
  auto lastPos = this->m_position;
 
  
  if(m_onGround)
      m_velocity.y=0;
  else
       ApplyGravity();
  
  m_position += m_velocity * timeStep;

  if(!m_onGround && IsOnGround()) //we just fell down, unstuck from ground
    { 
      m_onGround = true;
      while(IsOnGround())
	m_position += glm::vec3(0,0.001,0);
      m_position -= glm::vec3(0,0.001,0);
    }
  else if(m_onGround && !IsOnGround())
    m_onGround = false;
  
  m_cam->SetPosition(m_position+m_eyeOffset);
}
  
void Player::ApplyGravity()
{
  m_velocity += glm::vec3(0,-0.98,0);
  
  if(m_velocity.y<-50)
    m_velocity.y=-50;
}

bool Player::Jump(float velocity)
{
  if(m_onGround){
    m_position.y+=0.01;
    m_velocity.y = velocity;
    m_onGround = false;
  }
}

bool Player::OnGround()
{
  return m_onGround;
}

bool Player::IsOnGround()
{
  auto pos = this->m_position;
  pos+=glm::vec3(0,-0.001f,0);
  AABB g = m_aabb;
  g.Translate(pos);
  
  return m_octree->CheckCollisionB(g);
}


bool Player::IsColliding()
{
  AABB aabb = m_aabb;
  aabb.Translate(m_position);
  return m_octree->CheckCollisionB(aabb);
}
