# CMakeLists.txt dentro de ImageToAsciiGUI

cmake_minimum_required(VERSION 3.10)
project(ImageToAsciiGUI)

# Habilitar AUTOMOC para processar Q_OBJECT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)  # Opcional, para .ui (se usado no futuro)
set(CMAKE_AUTORCC ON)  # Opcional, para .qrc (se usado no futuro)

# Incluir o diretório atual para encontrar Main.moc
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Adicionar o executável
add_executable(ImageToAsciiGUI Main.cpp)

# Linkar com Qt5
target_link_libraries(ImageToAsciiGUI PRIVATE Qt5::Widgets)
