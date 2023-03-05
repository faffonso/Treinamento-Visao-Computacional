set(SOURCES assets.cpp)
add_library(assets ${SOURCES})
target_link_libraries(assets ${OpenCV_LIBS})