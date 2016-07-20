#ifndef MNODE_UTIL
#define MNODE_UTIL


const uint32_t F_EMPTY = 0; // all edges in the range (0,0)
const uint32_t F_SOLID = 0x80808080; // all edges in the range (0,8)

// DIM: X=0 Y=1 Z=2.
const int32_t R[3] = {1, 2, 0}; // row
const int32_t C[3] = {2, 0, 1}; // col
const int32_t D[3] = {0, 1, 2}; // depth


const glm::detail::tvec3<uint8_t> cubecoords[8] = // verts of bounding cube
{
	glm::detail::tvec3<uint8_t>(8, 8, 0),
	glm::detail::tvec3<uint8_t>(0, 8, 0),
	glm::detail::tvec3<uint8_t>(0, 8, 8),
	glm::detail::tvec3<uint8_t>(8, 8, 8),
	glm::detail::tvec3<uint8_t>(8, 0, 8),
	glm::detail::tvec3<uint8_t>(0, 0, 8),
	glm::detail::tvec3<uint8_t>(0, 0, 0),
	glm::detail::tvec3<uint8_t>(8, 0, 0),
};

const uint8_t fv[6][4] = // indexes for cubecoords, per each vert of a face orientation
{
	{ 2, 1, 6, 5 },
	{ 3, 4, 7, 0 },
	{ 4, 5, 6, 7 },
	{ 1, 2, 3, 0 },
	{ 6, 1, 0, 7 },
	{ 5, 4, 3, 2 },
};

// creates mask for bit of given dimension
inline uint8_t octaDim(uint8_t d)
{
	return 1<<d;
} 

inline uint8_t octaCoord(uint8_t d, uint8_t i) 
{
	return i & octaDim(d) >> d;
}

inline uint8_t edgeMake(uint8_t a, uint8_t b)
{
    return b << 4|a;
}

inline uint8_t oppositeOcta(uint8_t d, uint8_t i)
{
	return i ^ octaDim(D[d]);
}

inline uint8_t octaIndex(uint8_t d, uint8_t x, uint8_t y, uint8_t z)
{
	return (z<<D[d]) + (y<<C[d]) + (x<<R[d]);
}

inline uint8_t octaStep(uint8_t x, uint8_t y, uint8_t z, uint8_t scale) 
{
	return (((z>>scale)&1)<<2) | (((y>>scale)&1)<<1) | ((x>>scale)&1);
}

#endif