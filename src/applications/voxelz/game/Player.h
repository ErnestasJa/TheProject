#ifndef PLAYER_H
#define PLAYER_H

#include "Physics/CollisionObject.h"

#define PLAYER_HEIGHT 2.f
#define PLAYER_WIDTH 1.f

class ChunkManager;

class Player:public CollisionObject
{
    public:
        Player(ChunkManager* chunkManager, const glm::vec3 &feetPos);
        virtual ~Player();

        void Update(float dt);
        void Render(float dt);
    protected:
    private:
        ChunkManager* _chunkManager;
        VoxelMesh* _playerMesh;
        glm::vec3 _playerPos;
        SuperChunkPtr _oldSuperChunk;
        ChunkPtr _oldChunk;
};

Player::Player(ChunkManager* chunkManager, const glm::vec3 &feetPos)
{
    CollisionObject(glm::vec3(feetPos.x,feetPos.y,feetPos.z),glm::vec3(PLAYER_WIDTH/2.f,PLAYER_HEIGHT/2.f,PLAYER_WIDTH/2.f));
    _chunkManager=chunkManager;
}

Player::~Player()
{

}

void Player::CheckCollisions(dt)
{
    glm::ivec3 superchunkCoords=WorldToSuperChunkCoords(_playerPos),chunkCoords=WorldToChunkCoords(_playerPos);
    SuperChunkPtr superchunk=_chunkManager->GetSuperChunk(superchunkCoords);
    ChunkPtr chunk=superchunk->GetChunk(chunkCoords);


}

void Player::Update(float dt)
{
    Translate(glm::vec3(0,-9.81f,0)*dt);
    CheckCollisions(dt);
}

#endif // PLAYER_H
