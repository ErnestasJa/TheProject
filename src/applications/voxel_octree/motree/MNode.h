#ifndef MNODE_H
#define MNODE_H

#include <boost/move/move.hpp>

/**
Think of efficient way to store color data.
**/

struct MNode
{
private:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(MNode)

public:
    uint32_t start;
    uint8_t r,g,b;
    uint8_t size;

    MNode(BOOST_RV_REF(MNode) n);
    MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize=1);
    MNode(uint32_t morton, uint8_t nodeSize=1, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0);
    MNode();

    MNode& operator=(BOOST_RV_REF(MNode) x);
    bool operator<(const MNode &other) const;
    bool operator==(const MNode &other) const;

    inline uint32_t range_end() const;
};

#endif
