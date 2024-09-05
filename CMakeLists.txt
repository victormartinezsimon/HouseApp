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
    src/WebParserConfig.cpp
    src/Executor.cpp
    src/WebParser.cpp
    src/GeneralConfig.cpp
)

set(HEADERS
    src/DatabaseConnector.h
    src/WebDownloader.h
    src/WebParserConfig.h
    src/Executor.h
    src/WebParser.h
    src/GeneralConfig.h
)

# Crea un grupo para los archivos .h en Visual Studio
source_group("Headers" FILES ${HEADERS})


# Añade el ejecutable
add_executable(HouseApp ${SOURCES} ${HEADERS})

# Incluir directorios de cabeceras
target_include_directories(HouseApp PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Especificar el uso de C++17
target_compile_features(HouseApp PRIVATE cxx_std_20)

# Especifica la carpeta fuente y la carpeta de destino
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/Config")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}")

# Comando para copiar los archivos de source_dir a destination_dir
file(COPY "${SOURCE_DIR}" DESTINATION "${DESTINATION_DIR}")
# Mensaje para confirmar que la copia se realizó
message(STATUS "Archivos copiados de ${SOURCE_DIR} a ${DESTINATION_DIR}")

#sqlite3
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)
#target_link_libraries(HouseApp PRIVATE SQLiteCpp sqlite3)

find_package(SQLiteCpp CONFIG REQUIRED)
target_link_libraries(HouseApp PRIVATE SQLiteCpp)

find_package(CURL REQUIRED)
target_link_libraries(HouseApp PRIVATE CURL::libcurl)

find_package(LibXml2 REQUIRED)
target_link_libraries(HouseApp PRIVATE LibXml2::LibXml2)

find_package(RapidJSON CONFIG REQUIRED)
  target_link_libraries(HouseApp PRIVATE rapidjson)

