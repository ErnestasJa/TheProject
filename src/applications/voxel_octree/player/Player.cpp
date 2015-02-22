#include "Precomp.h"
#include "Player.h"
#include "motree/CollisionInfo.h"

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

/*
bool Player::IsSweptColliding(float timeStep)
{
  glm::vec3 vel = m_velocity*timeStep;

  if(IsNullVec(vel))
    return false;

  AABB g = m_aabb;
  g.SetCenter(this->m_position);
  
  auto collisions = m_octree->CheckCollisionSwept(g, vel);

  if(collisions.size()==0)
    {
      m_position += vel;
      return false;
    }
  
  glm::vec3 sweepDir = glm::normalize(vel);
  float minHitDist = collisions[0].time;
  bool axisBlocked[3] = {false,false,false};
  float minDistAxis[3] = {FLT_MAX, FLT_MAX, FLT_MAX};
  uint32_t axisVoxel[3] = {0,0,0};  

  ///for decoding voxel coordinates
  uint32_t x1,y1,z1;
  uint32_t x2,y2,z2;

  ///check which axis to clamp motion on
  for(unsigned int i = 0; i < collisions.size(); i++)
  {
    auto collision = collisions[i];
    
    if(collision.time==1||IsNullVec(collision.normal))
      continue;

    int axis = GetBlockAxis(collision.normal);
    
    if( collision.time < minHitDist)
      minHitDist = collision.time;

    decodeMK(collision.voxelMK,x1,y1,z1);

    loop(j, collisions.size())
      {
	if(i==j)
	  continue;
	
	decodeMK(collisions[i].voxelMK,x1,y1,z1);
	decodeMK(collisions[j].voxelMK,x2,y2,z2);
	
	if(axis==0&&z1==z2)
	  goto noblock;
	else if(axis==2&&x1==x2)
	  goto noblock;
	else if(axis==1 && (x1==x2 && z1==z2))
	  goto noblock;
      }
    axisBlocked[axis] = true;

  noblock:;
  }
  
  printf("Blck axis [%i, %i, %i]\n", axisBlocked[0], axisBlocked[1], axisBlocked[2]);
 
  glm::vec3 clampedMotion = vel;
  for(int i = 0; i < 3; i++)
  {
    if(axisBlocked[i])
      clampedMotion[i] *= minHitDist;      
  }

  m_position = m_position + clampedMotion;
  return true;
  }*/

 
