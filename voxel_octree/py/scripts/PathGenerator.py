class Vec3:
	def __init__(self, x, y, z):
		self.x = int(x)
		self.y = int(y)
		self.z = int(z)

	def Mul(self, scalar):
		return Vec3(self.x * scalar, self.y * scalar, self.z * scalar)

	def Sub(self, other):
		return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)

	def Norm(self):
		if self.x != 0:
			self.x = 1 if self.x > 0 else -1
		
		if self.y != 0:
			self.y = 1 if self.y > 0 else -1

		if self.z != 0:
			self.z = 1 if self.z > 0 else -1

	def Inverse(self):
		return Vec3(-self.x, -self.y, -self.z)

	def Equal(self, other):
		return self.x == other.x and self.y == other.y and self.z == other.z

	def ToStr(self):
		return "[" + str(self.x) + ", " + str(self.y) + ", " + str(self.z) + "]"

import random
class Path:
	def __init__(self, startPoint):
		self.path_initial_elevation = startPoint.y
		self.path = []
		self.AddPoint(startPoint.x,startPoint.z)

	def AddPoint(self, x, y):
		self.path.append(Vec3(x, self.path_initial_elevation, y))

	def GetLastPoint(self):
		return self.path[len(self.path)-1];

	def AppendSection(self, length, dir_vec3):
		dir_vec3.Norm()
		last = self.GetLastPoint()
		scaled_dir = dir_vec3.Mul(length)
		self.AddPoint(last.x + scaled_dir.x, last.z + scaled_dir.z)

	def GetRandomDirection(self, prev):
		switcher = {
			1: Vec3(1,0,0),
			2: Vec3(-1,0,0),
			3: Vec3(0,0,1),
			4: Vec3(0,0,-1)
	    }

		while True:
			dir = switcher.get(random.randint(1,4), Vec3(1,0,0))
			if dir.Inverse().Equal(prev) == False:
				return dir

from cpputils import cout
import octree

def RandomVoxelColor():
	octree.set_voxel_color(random.randint(255), random.randint(255), random.randint(255))

class PathVoxelizer:
	def __init__(self, path, halfWidth):
		self.path = path.path
		self.halfWidth = halfWidth
		self.last_dir = None

	def Voxelize(self):
		length = len(self.path)

		for i in range(1, length):
			start = self.path[i-1]
			end = self.path[i]

			self.VoxelizeSection(start, end)


	def VoxelizeSection(self, start, end):
		octree.set_voxel_color(random.randint(0,255),random.randint(0,255),random.randint(0,255))
	
		#Need to categorize direction changes and act according to that.
		extra_width = 0
		dir = end.Sub(start)

		if self.last_dir != None:
			if dir.Equal(self.last_dir) == False:
				extra_width = self.halfWidth

		self.last_dir = dir

		if dir.x != 0:
			startp = min(start.x, end.x) - extra_width
			endp = max(start.x, end.x) + extra_width

			for i in range(startp, endp+1):
				self.OutputVoxelsX(Vec3(i, start.y, start.z))

		elif dir.z != 0:
			startp = min(start.z, end.z) - extra_width
			endp = max(start.z, end.z) + extra_width

			for i in range(startp, endp + 1):
				self.OutputVoxelsZ(Vec3(start.x,start.y,i))

	def OutputVoxelsX(self, position):
		start = position.z - self.halfWidth
		end = position.z + self.halfWidth

		for x in range(start, end+1):
			self.OutputVoxel(Vec3(position.x, position.y, x))

			#Add walls
			if x == start or x == end:
				for i in range(1,4):
					self.OutputVoxel(Vec3(position.x, position.y+i, x))

	def OutputVoxelsZ(self, position):
		start = position.x - self.halfWidth
		end = position.x + self.halfWidth

		for x in range(start, end+1):
			self.OutputVoxel(Vec3(x, position.y, position.z))

			#Add walls
			if x == start or x == end:
				for i in range(1,4):
					self.OutputVoxel(Vec3(x, position.y+i, position.z))
			
	def OutputVoxel(self, position):
		octree.add_voxel(position.x, position.y, position.z)

