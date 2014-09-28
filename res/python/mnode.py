import octree

class MNode:
    def __init__(self, mk = 0):
        self.start = mk
        self.size = 1
        
    @classmethod
    def fromXYZ(cls, x, y, z):
        return cls(octree.encode_mk(x,y,z))

    def __lt__(self, other):
        return size > other.size if start == other.start else start < other.start

    def __eq__(self, other):
        return start == other.start and size == other.size
    
    def range_end(self):
        return start + (1 << (size * 3))
