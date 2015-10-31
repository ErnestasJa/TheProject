from PathGenerator import *

start = Vec3(200,0,200) 


path = Path(start)

dir = Vec3(-1,0,0)

dirs = {
	0: Vec3(1,0,0),
	1: Vec3(1,0,0)
}


for i in range(50):
	dir = dirs.get(i%2==0, Vec3(1,0,0))
	path.AppendSection(20, dir)

pathGen = PathVoxelizer(path,2)
pathGen.Voxelize()

cout("Voxelization done. Generating mesh..")

octree.add_voxel(start.x, 19, start.z)
octree.set_player_position(start.x, 40, start.z)
octree.generate_octree_meshes()