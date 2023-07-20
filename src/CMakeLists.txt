set(NAME_EXECUTABLE "bulk_server")
set(LIB_NAME "async")
set(project "bulk_server")

find_package(Boost REQUIRED)

include_directories(${NAME_EXECUTABLE}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/server
                    ${CMAKE_CURRENT_SOURCE_DIR}/async
                    )

add_library(${LIB_NAME} SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/async/async.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/async/async.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/server/server.cpp
            )

if(MINGW)
target_link_libraries(${LIB_NAME} PRIVATE
        ws2_32
        wsock32
        )
endif()

add_executable(${NAME_EXECUTABLE} main.cpp)

if(MINGW)
target_link_libraries(${NAME_EXECUTABLE}
                      ${LIB_NAME}
                      ws2_32
                      wsock32
                     )
endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


