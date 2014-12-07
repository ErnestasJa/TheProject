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

const AABB & Player::GetAABB()
{
    return m_aabb;
}

void Player::Update(float timeStep)
{
  auto lastPos = this->m_position;

  ApplyGravity();
  
  if(m_onGround)
    m_velocity.y=0;
  
  IsSweptColliding(timeStep);
  
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
bool Player::IsSweptColliding(float timeStep)
{
  AABB g = m_aabb;
  g.SetCenter(this->m_position);
  glm::vec3 vel = m_velocity*timeStep;

  if(IsNullVec(vel))
    return false;

  auto vec = m_octree->CheckCollisionSwept(g, vel);

  if(vec.size()==0)
    {
      m_position += vel;
      return false;
    }
  
  glm::vec3 vSweepDir = glm::normalize(vel);
  float fMinHitDist = vec[0].time;
  bool bAxisBlocked[3] = {false,false,false};
  float minDistAxis[3] = {99999999,99999999,99999999};
  uint32_t axisVoxel[3] = {0,0,0};  

  
  ///Unblock axis if hitting voxel edge that is a part of wall
  uint32_t x1,y1,z1;
  uint32_t x2,y2,z2;

  for(unsigned int i = 0; i < vec.size(); i++)
  {
    auto pEntity = vec[i];
    float t = pEntity.time;
    
    if(pEntity.time==1||IsNullVec(pEntity.normal))
      continue;

    int axis = GetBlockAxis(pEntity.normal);
    
    // get nearest distance
    if( t < fMinHitDist)
      fMinHitDist = t;

    bAxisBlocked[axis] = true;
  }

  glm::vec3 vBlockedMotion = vSweepDir * fMinHitDist;
  printf("Blck axisd[%.2f,%.2f,%.2f]\n", minDistAxis[0], minDistAxis[1], minDistAxis[2]);
  printf("Blck axis [%i, %i, %i]\n", bAxisBlocked[0], bAxisBlocked[1], bAxisBlocked[2]);


  /*if(bAxisBlocked[0]&&bAxisBlocked[2])
    {
      if(x1==x3)
	bAxisBlocked[2]=false;
      else if(z1==z3)
	bAxisBlocked[0]=false;
    }

  if(bAxisBlocked[0]&&bAxisBlocked[1])
    {
      if(x1==x2)
	bAxisBlocked[1]=false;
      else if(y1==y2)
	bAxisBlocked[0]=false;
    }

  
  if(bAxisBlocked[2]&&bAxisBlocked[1])
    {
      if(z2==z3)
	bAxisBlocked[1]=false;
      else if(y2==y3)
	bAxisBlocked[2]=false;
	}*/
  
  glm::vec3 vClampedMotion = vel;
  for(int i = 0; i < 3; i++)
  {
    if(bAxisBlocked[i]){
      vClampedMotion[i] *= fMinHitDist; //vBlockedMotion[i];
      
    }
  }

  m_position = m_position + vClampedMotion;
  return true;
}

 
