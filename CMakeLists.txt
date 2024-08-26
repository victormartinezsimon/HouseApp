cmake_minimum_required(VERSION 3.12)

# Nombre del proyecto
project(HouseProject)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configurar el compilador para Linux
if(UNIX AND NOT MSVC)
    # Forzar el uso de GCC
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    message(STATUS "Usando GCC en Linux")
endif()

# Configurar el compilador para Visual Studio
if(MSVC)
    message(STATUS "Compilando para Visual Studio")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Añade los archivos fuente y de cabecera
set(SOURCES
    src/main.cpp
    src/DatabaseConnector.cpp
    src/WebDownloader.cpp
)

set(HEADERS
    src/DatabaseConnector.h
    src/WebDownloader.h
)

# Crea un grupo para los archivos .h en Visual Studio
source_group("Headers" FILES ${HEADERS})

# Añade el ejecutable
add_executable(HouseApp ${SOURCES} ${HEADERS})

# Añadir el ejecutable (compilar main.cpp)
#add_executable(HouseApp main.cpp DatabaseConnector.cpp)

# Incluir directorios de cabeceras
target_include_directories(HouseApp PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Especificar el uso de C++17
target_compile_features(HouseApp PRIVATE cxx_std_20)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)

target_link_libraries(HouseApp
  SQLiteCpp
  sqlite3
  )

