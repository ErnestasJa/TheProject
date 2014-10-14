#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

template <int Depth>
class MortonOctTree;

typedef std::shared_ptr<MortonOctTree<10>> MortonOctTreePtr;

class CollisionManager
{
    public:
        CollisionManager(MortonOctTreePtr octree);
        virtual ~CollisionManager();
    protected:
        MortonOctTreePtr m_octree;
};

#endif // COLLISIONMANAGER_H
