cmake_minimum_required(VERSION 3.10)
project(MyGLFWProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加项目自己的include目录
include_directories("${CMAKE_SOURCE_DIR}/include")
message(STATUS "Added project include directory: ${CMAKE_SOURCE_DIR}/include")

# 自动扫描thirdParty目录下的所有库
file(GLOB THIRD_PARTY_DIRS "${CMAKE_SOURCE_DIR}/thirdParty/*")

# 遍历所有第三方库目录
foreach(LIB_DIR ${THIRD_PARTY_DIRS})
    if(IS_DIRECTORY ${LIB_DIR})
        # 添加include目录（如果存在）
        if(EXISTS "${LIB_DIR}/include")
            include_directories("${LIB_DIR}/include")
            message(STATUS "Added include directory: ${LIB_DIR}/include")
        endif()
        
        # 添加lib目录（支持多种命名方式）
        if(EXISTS "${LIB_DIR}/lib-mingw-w64")
            link_directories("${LIB_DIR}/lib-mingw-w64")
            message(STATUS "Added library directory: ${LIB_DIR}/lib-mingw-w64")
        elseif(EXISTS "${LIB_DIR}/lib")
            link_directories("${LIB_DIR}/lib")
            message(STATUS "Added library directory: ${LIB_DIR}/lib")
        endif()
    endif()
endforeach()

# 编译GLAD源文件
add_library(glad STATIC "${CMAKE_SOURCE_DIR}/thirdParty/glad/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/thirdParty/glad/include")

# 自动收集src目录下的所有.cpp文件
file(GLOB PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
message(STATUS "Found source files: ${PROJECT_SOURCES}")

# 添加可执行文件
add_executable(main 
    main.cpp
    ${PROJECT_SOURCES}
)

# 链接库（现在不需要GLFW和GLAD了，只需要Windows系统库）
# target_link_libraries(main glad glfw3dll)

# Windows平台需要链接的系统库
if(WIN32)
    target_link_libraries(main gdi32)
endif()

# 自动复制所有DLL文件到可执行文件目录
file(GLOB_RECURSE ALL_DLLS "${CMAKE_SOURCE_DIR}/thirdParty/*/*.dll")
foreach(DLL_FILE ${ALL_DLLS})
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_FILE}"
        $<TARGET_FILE_DIR:main>
    )
    message(STATUS "Will copy DLL: ${DLL_FILE}")
endforeach()

