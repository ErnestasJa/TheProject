#include "Precomp.h"
#include "MNode.h"
#include "Morton.h"

MNode::MNode(BOOST_RV_REF(MNode) n): start(n.start), size(n.size), r(n.r), g(n.g), b(n.b) {}

MNode& MNode::operator=(BOOST_RV_REF(MNode) x) // Move assign
{
    start = x.start;
    r = x.r;
    g = x.g;
    b = x.b;
    size  = x.size;
    solidFaces();
    return *this;
}

MNode::MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize)
{
    start = encodeMK(x,y,z);
    size = nodeSize;
    r = g = b = 255;
    solidFaces();
}

MNode::MNode(uint32_t morton, uint8_t nodeSize, uint8_t red, uint8_t green, uint8_t blue)
{
    start = morton;
    size = nodeSize;
    r = red;
    g = green;
    b = blue;
    solidFaces();
}

MNode::MNode(uint32_t morton, uint8_t nodeSize)
{
    start = morton;
    size = nodeSize;
    solidFaces();
}

MNode::MNode()
{
    start = 0;
    size = 0;
    r = g = b = 255;
    solidFaces();
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
