#this file is automatically generated 

cmake_minimum_required(VERSION 2.8)

SET(TARGET_SRC main.cpp )
#SET(TARGET_ADDED_LIBRARIES osgGA )
#### end var setup  ###
#SETUP_EXAMPLE(osgplanets)

link_directories(/usr/local/lib64)

add_executable(sgp2 main.cpp world.cpp enviroment.cpp util.cpp carrousel.cpp bumpercars.cpp threadmil.cpp abstracttypes.cpp animationcallbacks.cpp userinputhandlers.cpp particle.cpp)

target_link_libraries(sgp2 osgDB)
target_link_libraries(sgp2 osgGA)
target_link_libraries(sgp2 osgParticle)
target_link_libraries(sgp2 osgPresentation)
target_link_libraries(sgp2 osgShadow)
target_link_libraries(sgp2 osgSim)
target_link_libraries(sgp2 osg)
target_link_libraries(sgp2 osgTerrain)
target_link_libraries(sgp2 osgText)
target_link_libraries(sgp2 osgUtil)
target_link_libraries(sgp2 osgViewer)
target_link_libraries(sgp2 osgVolume)
