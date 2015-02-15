#ifndef GREEDYMESHBUILDER_H_INCLUDED
#define GREEDYMESHBUILDER_H_INCLUDED

class GreedyMeshBuilder
{
private:
    struct MaskNode
    {
        BOOST_MOVABLE_BUT_NOT_COPYABLE(MaskNode)

        uint8_t exists;
        intRGBA color;

        MaskNode()
        {
            exists=false;
            color=VecRGBToIntRGB(u8vec3(255));
        }

        MaskNode & operator = (bool value)
        {
            exists = value;
            return *this;
        }

        operator bool()
        {
            return exists==1;
        }
    };

    template<typename T>
    static void _clearMask(MaskNode **mask, T size);
    static void _clearMaskRanged(MaskNode **mask,int sx,int sy,int ex,int ey);
    template<typename T>
    static uint32_t _quadLength(uint32_t x, uint32_t y, MaskNode **mask,T size);
    template<typename T>
    static uint32_t _quadHeight(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask,T size);

public:
    template <typename T>
    static void GreedyBuild(T obj);
    template <typename T>
    static void AddQuadToMesh(T obj,const glm::ivec3 * face, const intRGBA & col);
    template <typename T>
    static void GetElement(T obj);
};
#endif // GREEDYMESHBUILDER_H_INCLUDED
