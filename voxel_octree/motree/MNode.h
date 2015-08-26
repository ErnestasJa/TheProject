#ifndef MNODE_H
#define MNODE_H

#include <boost/move/move.hpp>
#include "MNodeUtil.h"

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

    union
        {
        uint8_t edges[12];
        uint32_t faces[3];
    };

    MNode(BOOST_RV_REF(MNode) n);
    MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize=1);
    MNode(uint32_t morton, uint8_t nodeSize, uint8_t red, uint8_t green, uint8_t blue);
    MNode(uint32_t morton, uint8_t nodeSize=1);
    MNode();

    MNode& operator=(BOOST_RV_REF(MNode) x);
    bool operator<(const MNode &other) const;
    bool operator==(const MNode &other) const;

    inline uint32_t range_end() const;

    inline bool isEmpty()
    {
        return faces[0]==F_EMPTY;
    }

    inline bool isEntirelySolid()
    {
        return faces[0]==F_SOLID && faces[1]==F_SOLID && faces[2]==F_SOLID;
    }
    
    inline void setFaces(uint32_t face)
    {
        faces[0] = faces[1] = faces[2] = face;
    }
    
    inline void solidFaces()
    {
        setFaces(F_SOLID);
    }
    
    inline void emptyFaces() 
    {
        setFaces(F_EMPTY);
    }
    
    inline void edgeSet(uint8_t edgeIndex , bool end, uint8_t val)
    {
        uint8_t & edge = edges[edgeIndex];
        edge = ((end) ? (edge&0xF)|((val)<<4) : (edge&0xF0)|(val));
    }

    inline uint8_t edgeGet(uint8_t edgeIndex, bool coord)
    {
        return (coord) ? edges[edgeIndex]>>4 : edges[edgeIndex]&0xF;
    }
    
    inline uint8_t getEdgeEnd(int8_t dimension, uint8_t x, uint8_t y)
    {
        return edges[(dimension<<2) + (y<<1) + x]>>4;
    }

    inline uint8_t getEdgeStart(int8_t dimension, uint8_t x, uint8_t y)
    {
        return edges[(dimension<<2) + (y<<1) + x]&0xF;
    }

    // dimension parameter values: 0 = x, 1 = y, 2 = z
    // x and y parameters tell which edge is it.
    inline uint8_t cubeEdgeIndex(uint8_t dimension, uint8_t x, uint8_t y)
    {
        return (dimension<<2) + (y<<1) + x;
    }
};

#endif
