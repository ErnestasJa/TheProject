#ifndef APP_FORWARD_DECL
#define APP_FORWARD_DECL

#include "ForwardDecl.h"


class BVoxLoader;
class CollisionManager;
class CubeMesh;
class MortonOctTree;
class Player;
class Renderer;
class VoxMeshManager;

typedef std::shared_ptr<BVoxLoader> BVoxLoaderPtr;
typedef std::shared_ptr<CollisionManager> CollisionManagerPtr;
typedef std::shared_ptr<CubeMesh> CubeMeshPtr;
typedef std::shared_ptr<MortonOctTree> MortonOctTreePtr;
typedef std::shared_ptr<Player> PlayerPtr;
typedef std::shared_ptr<Renderer> RendererPtr;
typedef std::shared_ptr<VoxMeshManager> VoxMeshManagerPtr;

#endif