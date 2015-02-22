#include "Precomp.h"
#include "MortonOctree.h"

inline bool myfunction (const MNode & i,const MNode & j)
{
    return (i.start<j.start);
}

inline bool myeqfunction (const MNode & i,const MNode & j)
{
    return (i.start==j.start);
}

void MortonOctTree::AddNode(MNode node)
{
    m_nodes.insert(boost::range::lower_bound(m_nodes, MNode(node.start)),boost::move(node));
}

void MortonOctTree::AddOrphanNode(MNode node)
{
    m_nodes.push_back(boost::move(node));
}

bool MortonOctTree::IsSorted()
{
    return boost::range::is_sorted(m_nodes, myfunction);
}

void MortonOctTree::SortLeafNodes()
{
    boost::range::sort(m_nodes, myfunction);
}

void MortonOctTree::RemoveDuplicateNodes()
{
    boost::erase(m_nodes, boost::unique<boost::return_found_end>(m_nodes,myeqfunction));
}

bool MortonOctTree::CheckNodeFloat(float x, float y, float z)
{
    if(x<0||y<0||z<0)
        return false;

    return CheckNode(x,y,z);
}

bool MortonOctTree::CheckNode(uint32_t x, uint32_t y, uint32_t z)
{
    MNode n(x,y,z,1);
    return boost::range::binary_search(boost::make_iterator_range(m_nodes.begin(),m_nodes.end()),n,myfunction);
}

#include "VoxelSide.h"
uint8_t MortonOctTree::GetVisibleSides(uint32_t x, uint32_t y, uint32_t  z, vector<MNode>::iterator nodeIt)
{
    uint8_t sides=ALL;

    static MNode n;
    n.size = 1;

    auto urange = boost::make_iterator_range(nodeIt,m_nodes.end());
    auto lrange = boost::make_iterator_range(m_nodes.begin(),nodeIt);

    n.start = encodeMK(x,y+1,z);
    if (boost::range::binary_search(urange,n,myfunction))
        RemoveBit(sides, TOP);

    n.start = encodeMK(x,y,z+1);
    if (boost::range::binary_search(urange,n,myfunction))
        RemoveBit(sides, FRONT);

    n.start = encodeMK(x+1,y,z);
    if (boost::range::binary_search(urange,n,myfunction))
        RemoveBit(sides, LEFT);

    n.start = encodeMK(x-1,y,z);
    if (boost::range::binary_search(lrange,n,myfunction))
        RemoveBit(sides, RIGHT);

    n.start = encodeMK(x,y,z-1);
    if (boost::range::binary_search(lrange,n,myfunction))
        RemoveBit(sides, BACK);

    n.start = encodeMK(x,y-1,z);
    if (boost::range::binary_search(lrange,n,myfunction))
        RemoveBit(sides, BOTTOM);

    return sides;
}

vector<MNode> & MortonOctTree::GetChildNodes()
{
    return m_nodes;
}