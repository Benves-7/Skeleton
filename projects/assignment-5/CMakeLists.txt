#--------------------------------------------------------------------------
# assignment-5 project
#--------------------------------------------------------------------------

PROJECT(assignment-5)
FILE(GLOB assignment-5_headers code/*.h)
FILE(GLOB assignment-5_sources code/*.cc code/*.cpp)
FILE(GLOB assignment-5_shaders code/Shaders/*.shader)
FILE(GLOB assignment-5_textures code/Texture/*.jpg)
FILE(GLOB assignment-5_obj code/Obj/*.obj)

SET(files_assignment-5 ${assignment-5_headers} ${assignment-5_sources})
SOURCE_GROUP("assignment-5" FILES ${files_assignment-5})

ADD_EXECUTABLE(assignment-5 ${files_assignment-5})
TARGET_LINK_LIBRARIES(assignment-5 core render)
ADD_DEPENDENCIES(assignment-5 core render)