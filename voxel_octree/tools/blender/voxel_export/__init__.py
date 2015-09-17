bl_info = {
    "name": "Binary mesh voxelizer",
    "author": "Me",
    "blender": (2, 74, 0),
    "location": "File > Import-Export",
    "description": "Import-Export binary voxel map",
    "warning": "Put on the welding glasses before using this script.",
    "wiki_url": "http://www.feelsbadman.com/",
    "support": 'OFFICIAL',
    "category": "Import-Export"}

# Copyright (C) 2015: Ernestas Januševičius, ernestasjanusevicius@gmail.com

if "bpy" in locals():
    import importlib
    if "voxel_export" in locals():
        importlib.reload(voxel_export)

import os
import bpy
from bpy.props import (
        CollectionProperty,
        StringProperty,
        BoolProperty,
        EnumProperty,
        FloatProperty,
        )
from bpy_extras.io_utils import (
        ExportHelper,
        orientation_helper_factory,
        axis_conversion,
        )


VoxOrientationHelper = orientation_helper_factory("VoxOrientationHelper", axis_forward='Y', axis_up='Z')

class ExportBVOX(bpy.types.Operator, ExportHelper, VoxOrientationHelper):
    """Export a single object as a binary voxel map, """
    bl_idname = "export_mesh.bvox"
    bl_label = "Export BVOX"

    filename_ext = ".bvox"
    filter_glob = StringProperty(default="*.bvox", options={'HIDDEN'})

    use_mesh_modifiers = BoolProperty(
            name="Apply Modifiers",
            description="Apply Modifiers to the exported mesh",
            default=True,
            )
    use_normals = BoolProperty(
            name="Normals",
            description="Export Normals for smooth and "
                        "hard shaded faces "
                        "(hard shaded faces will be exported "
                        "as individual faces)",
            default=True,
            )
    use_uv_coords = BoolProperty(
            name="UVs",
            description="Export the active UV layer",
            default=True,
            )
    use_colors = BoolProperty(
            name="Vertex Colors",
            description="Export the active vertex color layer",
            default=True,
            )

    global_scale = FloatProperty(
            name="Scale",
            min=0.01, max=1000.0,
            default=1.0,
            )

    @classmethod
    def poll(cls, context):
        return context.active_object != None

    def execute(self, context):
        from . import voxel_export

        from mathutils import Matrix

        keywords = self.as_keywords(ignore=("axis_forward",
                                            "axis_up",
                                            "global_scale",
                                            "check_existing",
                                            "filter_glob",
                                            ))
        global_matrix = axis_conversion(to_forward=self.axis_forward,
                                        to_up=self.axis_up,
                                        ).to_4x4() * Matrix.Scale(self.global_scale, 4)
        keywords["global_matrix"] = global_matrix

        filepath = self.filepath
        filepath = bpy.path.ensure_ext(filepath, self.filename_ext)

        return voxel_export.save(self, context, **keywords)

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.prop(self, "use_mesh_modifiers")
        row.prop(self, "use_normals")
        row = layout.row()
        row.prop(self, "use_uv_coords")
        row.prop(self, "use_colors")

        layout.prop(self, "axis_forward")
        layout.prop(self, "axis_up")
        layout.prop(self, "global_scale")


def menu_func_export(self, context):
    self.layout.operator(ExportBVOX.bl_idname, text="BinaryVox (.bvox)")


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()
