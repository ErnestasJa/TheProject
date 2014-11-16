#include "Precomp.h"
#include "MNode.h"
#include "Morton.h"

MNode::MNode(BOOST_RV_REF(MNode) n): start(n.start), size(n.size) {}

MNode& MNode::operator=(BOOST_RV_REF(MNode) x) // Move assign
{
    start = x.start;
    size  = x.size;
    return *this;
}

MNode::MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize)
{
    start = encodeMK(x,y,z);
    size = nodeSize;
    contained_by = 0;
}

MNode::MNode(uint32_t morton, uint8_t nodeSize)
{
    start = morton;
    size = nodeSize;
    contained_by = 0;
}

MNode::MNode()
{
    size = 0;
}

inline uint32_t MNode::range_end() const
{
    return start + (uint32_t(1) << (size * 3));
}

bool MNode::operator<(const MNode &other) const  /// ordering: z order, plus on equal sizes largest first
{
    return start == other.start ? size > other.size : start < other.start;
}

bool MNode::operator==(const MNode &other) const
{
    return start == other.start && size == other.size;
}
