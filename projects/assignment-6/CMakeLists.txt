#--------------------------------------------------------------------------
# assignment-6 project
#--------------------------------------------------------------------------

PROJECT(assignment-6)
FILE(GLOB assignment-6_headers code/*.h)
FILE(GLOB assignment-6_sources code/*.cc code/*.cpp)
FILE(GLOB assignment-6_shaders code/Shaders/*.shader)
FILE(GLOB assignment-6_textures code/Texture/*.jpg code/Texture/*.png)
FILE(GLOB assignment-6_obj code/Obj/*.obj)

SET(files_assignment-6 ${assignment-6_headers} ${assignment-6_sources} ${assignment-6_shaders})
SOURCE_GROUP("assignment-6" FILES ${files_assignment-6})

ADD_EXECUTABLE(assignment-6 ${files_assignment-6})
TARGET_LINK_LIBRARIES(assignment-6 core render)
ADD_DEPENDENCIES(assignment-6 core render)
