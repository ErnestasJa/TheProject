#ifndef BLOCK_H
#define BLOCK_H

enum EBlockType
{
    EBT_AIR,

    EBT_STONE,
    EBT_DIRT,
    EBT_GRASS,
    EBT_SAND,
    EBT_WATER,
    EBT_WOOD,
    EBT_LEAF,
    EBT_VOIDROCK,

    EBT_COUNT
};

class Block
{
    public:
        Block()
        {
            m_active=false;
            m_blockType=EBT_AIR;
        }
        virtual ~Block(){};

        bool IsActive() const
        {
            return m_active;
        }
        void SetActive(bool active)
        {
            m_active=active;
        }
        void SetBlockType(EBlockType type)
        {
            m_blockType=type;
        }

        uint32_t GetBlockType() const
        {
            return m_blockType;
        }

        //static const int BLOCK_RENDER_SIZE=1;
    protected:
    private:

        uint32_t m_active:1;
        uint32_t m_blockType:31;
};

#endif // BLOCK_H
