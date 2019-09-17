#pragma once

#ifndef _GLOBALS_H
#define _GLOBALS_H

// WINDOW TITLES
#define CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE      "Empty app - OpenGL + GLFW Library initialization"
#define CAPP_PROGRA3_CUBES_WINDOW_TITLE         "Cubes app"
#define CAPP_PROGRA3_OBJ_LOADER_WINDOW_TITLE    ".OBJ Loader app"
#define CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE     "Geometric shapes app"
#define CAPP_PROGRA3_HEXGRID_WINDOW_TITLE       "HexGrid app"

// SHADER FILENAMES
#define SHADER_PROGRAM_COLOR_CUBE               "COLOR_CUBE_SHADER"
#define VERTEX_SHADER_3D_OBJECT_COLOR           "SHADERS\\glsl_testobject.vert"
#define FRAGMENT_SHADER_3D_OBJECT_COLOR         "SHADERS\\glsl_testobject.frag"

#define SHADER_PROGRAM_COLOR_OBJECT             "COLOR_OBJECT_SHADER"
#define VERTEX_SHADER_3D_OBJECT                 "SHADERS\\glsl_3dobject.vert"
#define FRAGMENT_SHADER_3D_OBJECT               "SHADERS\\glsl_3dobject.frag"

#define SHADER_PROGRAM_TEXTURED_OBJECT          "TEXTURED_OBJECT_SHADER"
#define VERTEX_SHADER_TEXTURED_3D_OBJECT        "SHADERS\\glsl_3dobject_textured.vert"
#define FRAGMENT_SHADER_TEXTURED_3D_OBJECT      "SHADERS\\glsl_3dobject_textured.frag"

#define SHADER_PROGRAM_COLOR_OBJECT_PHONG       "PHONG_COLOR_OBJECT_SHADER"
#define VERTEX_SHADER_3D_OBJECT_PHONG           "SHADERS\\glsl_3dobject_phong.vert"
#define FRAGMENT_SHADER_3D_OBJECT_PHONG         "SHADERS\\glsl_3dobject_phong.frag"

#define SHADER_PROGRAM_TEXTURED_OBJECT_PHONE    "PHONG_TEXTURED_OBJECT_SHADER"
#define VERTEX_SHADER_TEXTURE_3D_OBJECT_PHONG   "SHADERS\\glsl_3dobject_textured_phong.vert"
#define FRAGMENT_SHADER_TEXTURE_3D_OBJECT_PHONG "SHADERS\\glsl_3dobject_textured_phong.frag"

#define SHADER_PROGRAM_TEXTURED_CUBE            "TEXTURED_CUBE_SHADER"
#define VERTEX_SHADER_MC_CUBE                   "SHADERS\\glsl_mccube.vert"
#define FRAGMENT_SHADER_MC_CUBE                 "SHADERS\\glsl_mccube.frag"

#define SHADER_PROGRAM_MENU                     "MENU_SHADER"
#define VERTEX_SHADER_MENU                      "SHADERS\\glsl_menu.vert"
#define FRAGMENT_SHADER_MENU                    "SHADERS\\glsl_menu.frag"

#define SHADER_PROGRAM_WIREFRAME                "WIREFRAME_SHADER"
#define VERTEX_SHADER_WIREFRAME                 "SHADERS\\glsl_wireframe.vert"
#define FRAGMENT_SHADER_WIREFRAME               "SHADERS\\glsl_wireframe.frag"

// TEXTURE FILENAMES
#define MENU_TEXTURE_FILE                       "TEXTURES\\Menu.tga"
#define MENU_TEXTURE_FILE_EXAMEN_FINAL          "TEXTURES\\MenuExamenFinal.tga"

#define MC_GRASS_DIRT_TEXTURE                   "TEXTURES\\mc_grass_dirt_cube_512x384.tga"
#define MC_DIRT_TEXTURE                         "TEXTURES\\mc_dirt_cube_512x384.tga"
#define MC_DIRT_2_TEXTURE                       "TEXTURES\\mc_dirt2_cube_512x384.tga"
#define MC_GRASS_2_TEXTURE                      "TEXTURES\\mc_grass2_512x384.tga"
#define MC_GRASS_2_DIRT_TEXTURE                 "TEXTURES\\mc_grass2_dirt_512x384.tga"
#define MC_GRASS_3_TEXTURE                      "TEXTURES\\mc_grass4_512x384.tga"
#define MC_LEAVES_TEXTURE                       "TEXTURES\\mc_leaves_cube_512x384.tga"
#define MC_WOOD_TEXTURE                         "TEXTURES\\mc_wood_512x384.tga"

// 
#define DEFAULT_ROTATION_SPEED                  90.0f
#define DEFAULT_CAMERA_MOVE_SPEED               0.015f

// JSON CONFIGURATION FILE FOR HEX GRID
#define HEX_GRID_1_CFG_FILE                     "HEXGRID\\hexgrid_empty_cfg.json"
#define HEX_GRID_2_CFG_FILE                     "HEXGRID\\hexgrid_cfg.json"

#define HEX_GRID_KEY_HEXGRID                     "HexGrid"
#define HEX_GRID_KEY_NUMCOLS                     "numCols"
#define HEX_GRID_KEY_NUMROWS                     "numRows"
#define HEX_GRID_KEY_CELLSIZE                    "cellSize"
#define HEX_GRID_KEY_ORIENTATION                 "orientation"
#define HEX_GRID_KEY_ORIENTATION_FLAT            "flat"
#define HEX_GRID_KEY_ORIENTATION_POINTY          "pointy"
#define HEX_GRID_KEY_MODELS                      "Models"
#define HEX_GRID_KEY_MODEL_NAME                  "name"
#define HEX_GRID_KEY_MODEL_FILENAME              "filename"
#define HEX_GRID_KEY_MODEL_INSTANCES             "ModelInstances"
#define HEX_GRID_KEY_MODEL_INSTANCE_MODEL        "model"
#define HEX_GRID_KEY_MODEL_INSTANCE_COLUMN       "column"
#define HEX_GRID_KEY_MODEL_INSTANCE_ROW          "row"
#define HEX_GRID_KEY_MODEL_INSTANCE_SCALE        "scale"
#define HEX_GRID_KEY_MODEL_INSTANCE_ROTATION     "rotation"


#define CAPP_PROGRA3_CUBE "cube"
#define  CAPP_PROGRA3_ESFERA "esfera"

#endif // !_GLOBALS_H
