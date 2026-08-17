CXX = g++
CXXFLAGS = -std=c++14 -Wall -Iinclude -Iinclude/core -Iinclude/devices -Iinclude/patterns

# Lista de  los archivos fuente 
SRCS = src/main.cpp \
       src/devices/Devices.cpp \
       src/patterns/ZigbeeDeviceHandler.cpp \
       src/patterns/DeviceProvisioner.cpp \
       src/patterns/SensorEvents.cpp \
       src/patterns/SecuritySystem.cpp \
       src/patterns/DeviceAddons.cpp \
       src/patterns/SmartRoutine.cpp \
       src/core/HomeManager.cpp \
       src/ui/ConsoleUI.cpp

# Reemplaza la extensión .cpp por .o para los objetos
OBJS = $(SRCS:.cpp=.o)

# Nombre del ejecutable
TARGET = SmartHomeOS.exe

# Regla por defecto (la que se ejecuta al llamar a mingw32-make solo)
all: $(TARGET)

# Cómo enlazar los objetos para crear el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Cómo compilar cada archivo .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos compilados
clean:
	del /Q /S *.o
	del /Q $(TARGET)