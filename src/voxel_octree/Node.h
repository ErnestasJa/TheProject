#ifndef NODE_H
#define NODE_H

#include <assert.h>
#include <stdint.h>

struct vec3
{
    uint32_t x, y, z;

    vec3()
    {
        x=y=z=0;
    }

    vec3(uint32_t px, uint32_t py, uint32_t pz)
    {
        x = px; y = py; z = pz;
    }
};

static const uint8_t MAX_DEPTH_LEVEL = 16;
static const uint32_t DEPTH_HALF_TABLE[]={0,1,2,4,8,16,32,64,128,256,512,2048,4096,8192,16384,32768};    ///HALF NODE COUNT AT LEVEL X
static const uint32_t DEPTH_TABLE[]=     {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536}; ///NODE COUNT AT LEVEL X
static const uint32_t POSITION_MASK[]=   {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

struct Node
{
    Node * children[8];
    Node * parent;
    uint8_t data = 0;

    Node()
    {
        parent = nullptr;
        children[0] = children[1] = children[2] = children[3] =children[4] =children[5] = children[6] = children[7] = nullptr;
        data = 0;
    }

    Node * GetChild(vec3 pos, uint8_t currentDepthLevel)
    {
        if(currentDepthLevel == 0)
                return this;

        assert ((pos.x&~POSITION_MASK[currentDepthLevel])==0);
        assert ((pos.y&~POSITION_MASK[currentDepthLevel])==0);
        assert ((pos.z&~POSITION_MASK[currentDepthLevel])==0);

        uint8_t which=0;

        if(pos.x>=DEPTH_HALF_TABLE[currentDepthLevel])
            which +=1;
        if(pos.z>=DEPTH_HALF_TABLE[currentDepthLevel])
            which +=2;
        if(pos.y>=DEPTH_HALF_TABLE[currentDepthLevel])
            which +=4;

        if(children[which])
        {
            currentDepthLevel -= 1;
            pos.x &= POSITION_MASK[currentDepthLevel];
            pos.y &= POSITION_MASK[currentDepthLevel];
            pos.z &= POSITION_MASK[currentDepthLevel];
            return children[which]->GetChild(pos,currentDepthLevel);
        }

        return nullptr;
    }

    Node * AddChild(vec3 pos, uint8_t currentDepthLevel)
    {
        assert ((pos.x&~POSITION_MASK[currentDepthLevel])==0);
        assert ((pos.y&~POSITION_MASK[currentDepthLevel])==0);
        assert ((pos.z&~POSITION_MASK[currentDepthLevel])==0);

        uint8_t which=0;

        if(pos.x>=DEPTH_HALF_TABLE[currentDepthLevel])
            which +=1;
        if(pos.z>=DEPTH_HALF_TABLE[currentDepthLevel])
            which +=2;
        if(pos.y>=DEPTH_HALF_TABLE[currentDepthLevel])
            which +=4;


        if(currentDepthLevel == 1)
        {
            children[which] = new Node();
            return children[which];
        }

        if(children[which] == nullptr)
            children[which] = new Node();

        currentDepthLevel -= 1;
        pos.x &= POSITION_MASK[currentDepthLevel];
        pos.y &= POSITION_MASK[currentDepthLevel];
        pos.z &= POSITION_MASK[currentDepthLevel];
        return children[which]->AddChild(pos,currentDepthLevel);
    }

    typedef void (* TraverseAction) (Node * node);


    static void TraverseLeafNodes(Node * node, uint8_t currentDepth, TraverseAction action)
    {
        if(!node)
            return;

        if(currentDepth == 0)
            action(node);
        else
            for(uint8_t i = 0; i < 8; i++)
                TraverseLeafNodes(node->children[i], currentDepth - 1, action);
    }
};

#endif // NODE_H
