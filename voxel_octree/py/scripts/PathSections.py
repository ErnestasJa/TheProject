import octree
from cpputils import cout

PathHalfWidth = 3

class SectionType(Enum):
	XAxis = 1
	ZAxis = 2


class VoxelSection:
	def __init__(self, section_type):
		self.Type = section_type

class VoxelSectionX:
	def __init__(self, position, length, width, wall_height):
		super(VoxelSectionX, self).__init__(SectionType.XAxis)
		self.Position = position
		self.Length = length
		self.WallHeight = wall_height

	def Generate(self):
		end = self.Position.x + length

		for x in range(self.Position.x, end):
			startz = self.Position.z - PathHalfWidth
			endz = self.Position.z + PathHalfWidth

			for z in range(startz, endz + 1):
				octree.add_voxel(Vec3(x, self.Position.y ,z))
