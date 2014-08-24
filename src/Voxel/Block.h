#ifndef BLOCK_H
#define BLOCK_H

enum EBlockType
{
    EBT_DEFAULT,

    EBT_STONE,
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
            m_blockType=EBT_DEFAULT;
        }
        virtual ~Block(){};

        bool IsActive()
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

        static const int BLOCK_RENDER_SIZE=1;
    protected:
    private:
        bool m_active;

        EBlockType m_blockType;
};

#endif // BLOCK_H
