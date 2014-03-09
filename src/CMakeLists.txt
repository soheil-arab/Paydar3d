set(robotoos_SRC

Act.cpp
Attacker.cpp
bools.cpp
Deffender.cpp
Geom.cpp
Goalie.cpp
MainDecide.cpp
Math.cpp
matrix.cpp
Middle.cpp
Parser.cpp
Parser.h
Skill.cpp
Skill.h
Svd.cpp
WorldModel.cpp
ConnectionManager.cpp
Logger.cpp
#~ connection.cpp
line.cpp
Localize.cpp
)

set(robotoos_HEAD
Act.h
Decide.h
defines.h
Geom.h
gmath.h
Math.h
matrix.h
salt.h
Svd.h
tvector.h
rvdraw.h
Types.h
vector.h
WorldModel.h
ConnectionManager.h
Logger.h
#~ connection.h
line.h
Localize.h
)

set(robotoos_SRCS
    ${rcssnet_SRCS}
   ${robotoos_HEAD}
   ${robotoos_SRC}
   main.cpp
)

set(CMAKE_CXX_FLAGS " -pg -I/usr/include/eigen3 -lorocos-kdl")

include_directories(${SPARK_INCLUDE_DIR}  )

add_executable(Robotoos3D ${robotoos_SRCS})

target_link_libraries(Robotoos3D)

install(TARGETS   Robotoos3D        		DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(PROGRAMS  start.sh       		DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY ACT          DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(PROGRAMS  kill.sh        		DESTINATION ${CMAKE_SOURCE_DIR}/bin)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/ACT ${CMAKE_BINARY_DIR}/src/ACT)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/start.sh ${CMAKE_BINARY_DIR}/src/start.sh)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/kill.sh ${CMAKE_BINARY_DIR}/src/kill.sh)


