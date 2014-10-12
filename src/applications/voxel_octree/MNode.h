#ifndef MNODE_H
#define MNODE_H

#include <boost/move/move.hpp>

struct MNode
{
private:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(MNode)

public:
    uint32_t start;
    uint32_t contained_by : 24;
    uint32_t size : 8;

    MNode(BOOST_RV_REF(MNode) n);
    MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize=1);
    MNode(uint32_t morton, uint8_t nodeSize=1);
    MNode();

    MNode& operator=(BOOST_RV_REF(MNode) x);
    bool operator<(const MNode &other) const;
    bool operator==(const MNode &other) const;

    inline uint32_t range_end() const;
};

static inline bool myfunction (const MNode & i,const MNode & j)
{
    return (i.start<j.start);
}

static inline bool myeqfunction (const MNode & i,const MNode & j)
{
    return (i.start==j.start);
}

#endif
