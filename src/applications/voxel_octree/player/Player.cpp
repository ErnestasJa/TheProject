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
bool Player::IsSweptColliding(float timeStep)
{
  glm::vec3 vel = m_velocity*timeStep;

  if(IsNullVec(vel))
    return false;

  AABB g = m_aabb;
  g.SetCenter(this->m_position);
  
  auto vec = m_octree->CheckCollisionSwept(g, vel);

  if(vec.size()==0)
    {
      m_position += vel;
      return false;
    }
  
  glm::vec3 sweepDir = glm::normalize(vel);
  float minHitDist = vec[0].time;
  bool axisBlocked[3] = {false,false,false};
  float minDistAxis[3] = {99999999,99999999,99999999};
  uint32_t axisVoxel[3] = {0,0,0};  

  ///for decoding voxel coordinates
  uint32_t x1,y1,z1;
  uint32_t x2,y2,z2;

  ///check which axis to clamp motion on
  for(unsigned int i = 0; i < vec.size(); i++)
  {
    auto voxel = vec[i];
    float t = voxel.time;
    
    if(t==1||IsNullVec(voxel.normal))
      continue;

    int axis = GetBlockAxis(voxel.normal);
    
    // get nearest distance
    if( t < minHitDist)
      minHitDist = t;

    decodeMK(voxel.voxelMK,x1,y1,z1);
    printf("CI Axis:[%i] Time:[%0.3f] Normal[%.2f,%.2f,%.2f]\nVoxel[%i,%i,%i]\n",axis,t,voxel.normal.x,voxel.normal.y,voxel.normal.z,x1,y1,z1);

    ///this loop lets player slide against walls just fine, but can't make it work on y axis
    ///and this probably also will have more issues when player size changes and voxel count increases
    loop(j, vec.size())
      {
	if(i==j)
	  continue;
	
	decodeMK(vec[i].voxelMK,x1,y1,z1);
	decodeMK(vec[j].voxelMK,x2,y2,z2);
	if(axis==0&&z1==z2){
	  printf("Noblock X\n");
	  goto noblock;
	}
	else if(axis==2&&x1==x2){
	  printf("Noblock Z\n");
	  goto noblock;
	}
	else if(axis==1 && (x1==x2 && z1==z2))
	  {
	    printf("Noblock Y\n");
	    goto noblock;
	  }
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
}

 
