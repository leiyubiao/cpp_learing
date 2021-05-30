
set(MRT_PKG_VERSION 4.0.0)
# Modify only if you know what you are doing!
cmake_minimum_required(VERSION 3.5.1)
project(json1) # PROJECT_NAME: project（）里的名字，一般也就是包的名字

###################
## Find packages ##
###################
find_package(mrt_cmake_modules REQUIRED)
include(UseMrtStdCompilerFlags)
include(GatherDeps)


#set(PROJECT_MESSAGE_DEPENDS std_msgs geometry_msgs) # 添加消息，可有可无。
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/custom.cmake") # 添加用户自定义的.cmake文件
    include("${CMAKE_CURRENT_SOURCE_DIR}/custom.cmake")
endif()

find_package(AutoDeps REQUIRED COMPONENTS ${DEPENDEND_PACKAGES}) # 添加其它自定义的库时，比如包括自定义的消息类型时，需要在这里加吗？还是package.xml中加

mrt_parse_package_xml()

########################
## Add python modules ## 
########################
# This adds a python module if located under src/{PROJECT_NAME)
mrt_python_module_setup()

mrt_glob_files(PROJECT_PYTHON_SOURCE_FILES_SRC "python_api/*.cpp")
if (PROJECT_PYTHON_SOURCE_FILES_SRC)
    # Add a cpp-python api library. Make sure there are no name collisions with python modules in this project
    mrt_add_python_api( ${PROJECT_NAME}
        FILES ${PROJECT_PYTHON_SOURCE_FILES_SRC}
        )
endif()

################################################
## Declare ROS messages, services and actions ##
################################################

# Add message, service and action files
mrt_add_message_files(msg)
mrt_add_service_files(srv)
mrt_add_action_files(action)

# Generate added messages and services with any dependencies listed here
if (ROS_GENERATE_MESSAGES)
    generate_messages(
        DEPENDENCIES ${PROJECT_MESSAGE_DEPENDS}
        )
endif()

# Generate dynamic reconfigure options 添加用户用于调参的文件，可以是以下文件格式，也可以是.yaml 这样会有着色效果
mrt_glob_files(PARAMS_FILES "cfg/*.params" "cfg/*.cfg" "cfg/*.mrtcfg" "cfg/*.rosif") # 这句话的含义是PARAMS_FILES 中有cfg 文件中的所有这些格式结尾的文件
if (PARAMS_FILES)
    generate_ros_parameter_files(${PARAMS_FILES})
    generate_ros_interface_files(${PARAMS_FILES})
endif()

############################
## Read source code files ##  
############################
mrt_glob_files_recurse(PROJECT_HEADER_FILES_INC "include/*.h" "include/*.hpp" "include/*.cuh")# 把include 里面的所有.h .hpp .cuh声明为 PROJECT_HEADER_FILES_INC,递归调用？？？
mrt_glob_files(PROJECT_SOURCE_FILES_INC "src/*.h" "src/*.hpp" "src/*.cuh") 
mrt_glob_files(PROJECT_SOURCE_FILES_SRC "src/*.cpp" "src/*.cu")

###########
## Build ##
###########
# Declare a cpp library 会生成的cpp文件添加相关的头文件
mrt_add_library(${PROJECT_NAME}
    INCLUDES ${PROJECT_HEADER_FILES_INC} ${PROJECT_SOURCE_FILES_INC}
    SOURCES ${PROJECT_SOURCE_FILES_SRC}
    )


mrt_glob_folders(SRC_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src")
if (SRC_DIRECTORIES)
   # Found subfolders, add executable for each subfolder
    foreach(SRC_DIR ${SRC_DIRECTORIES})
      mrt_add_executable(${SRC_DIR} FOLDER "src/${SRC_DIR}") # 每一个节点自己定义一个文件夹
      mrt_add_node_and_nodelet(${SRC_DIR} FOLDER "src/${SRC_DIR}")
    endforeach()
else()
  # No subfolder found, add executable and python modules for src folder
     mrt_add_executable(${PROJECT_NAME} FOLDER "src")
     mrt_add_node_and_nodelet(${PROJECT_NAME} FOLDER "src")
 endif()


#############
## Install ##
#############
mrt_install(PROGRAMS scripts FILES launch rviz maps res data nodelet_plugins.xml plugin_description.xml ${PROJECT_INSTALL_FILES})

#############
## Testing ##
#############
# Add test targets for cpp and python tests
if (CATKIN_ENABLE_TESTING)
    mrt_add_ros_tests(test)
    mrt_add_tests(test)
    mrt_add_nosetests(test)
endif()
