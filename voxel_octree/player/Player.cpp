#include "Precomp.h"
#include "Player.h"
#include "motree/CollisionInfo.h"

#define GRAVITY_CONSTANT -0.5
#define JUMP_VELOCITY_CONSTANT 40

Player::Player(CameraPtr cam, CollisionManager * octree, glm::vec3 position, AABB aabb, glm::vec3 eyeOffset)
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

const AABB & Player::GetAABB()
{
    return m_aabb;
}

void Player::Update(float timeStep)
{
  ApplyGravity();
  
  if(m_onGround)
    m_velocity.y=0;
  
  IsSweptColliding(timeStep);
  
  if(!m_onGround && IsOnGround()) //we just fell down, unstuck from ground
    {
      m_onGround = true;
      /*while(IsOnGround())
	m_position += glm::vec3(0,0.001,0);
	m_position -= glm::vec3(0,0.001,0);*/
    }
  else if(m_onGround && !IsOnGround())
    m_onGround = false;
  
  m_cam->SetPosition(m_position+m_eyeOffset);
}
  
void Player::ApplyGravity()
{
  m_velocity += glm::vec3(0, GRAVITY_CONSTANT, 0);
  
  if(m_velocity.y<-50)
    m_velocity.y=-50;
}

bool Player::Jump(float velocity)
{
  if(m_onGround){
    m_position.y+=0.01;
    m_velocity.y = JUMP_VELOCITY_CONSTANT;
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
  g.SetCenter(pos);

  return m_octree->CheckCollisionB(g);
}


bool Player::IsColliding()
{
  AABB aabb = m_aabb;
  aabb.SetCenter(m_position);
  return m_octree->CheckCollisionB(aabb);
}

bool sortCI(const AABBCollisionInfo & a, const AABBCollisionInfo & b)
{
  return a.time<b.time;
}

int GetBlockAxis(const glm::vec3 & n)
{
  return (n.x!=0.0f?0:(n.y!=0.0f?1:2));
}

static bool IsNullVec(const glm::vec3 & n)
{
  return (n.x==n.y&&n.y==n.z&&n.z==0.0);
}

#include <stdio.h>
#include <float.h>

bool SortCollisions(AABBCollisionInfo & a, AABBCollisionInfo & b)
{
  return a.time<b.time;
}

/**
                The algorithm?

1) Get all collision 'points'
2) Sort by collision time
3) Move player according to first collision.
4) Check if still colliding. If so - return to step 1, else we good.. ?
x) Handle precission errors

**/

uint32_t MAX_ITERATIONS = 20;
bool Player::IsSweptColliding(float timeStep)
{
  ///for decoding voxel coordinates
  uint32_t x1,y1,z1;
  uint32_t x2,y2,z2;

  glm::vec3 velocity = m_velocity*timeStep;

  for (int i = 0; i < MAX_ITERATIONS; ++i)
  {  
    if(IsNullVec(velocity))
      return false;

    AABB g = m_aabb;
    g.SetCenter(this->m_position);
    
    auto collisions = m_octree->CheckCollisionSwept(g, velocity);
    std::sort(collisions.begin(), collisions.end(), SortCollisions);
      
    if(collisions.size()==0)
    {
      m_position += velocity;
      return false;
    }
    
    glm::vec3 sweepDir = glm::normalize(velocity);
    auto collision = collisions[0];
    float minHitDist = collision.time;
    int axis = GetBlockAxis(collision.normal);
    auto clampedMotion = velocity;
    clampedMotion[axis] *= minHitDist;
    velocity = clampedMotion;
  }
  m_position = m_position + velocity;
  return true;
}
