# Имя выходного файла
OUTPUT = lab3

# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -Wall

# Все исходные файлы
SOURCES = md5.cpp lab3.cpp lab1_lib.cpp rsa_lib.cpp gamal_lib.cpp gost.cpp

# Объектные файлы (с заменой .cpp на .o)
OBJECTS = $(SOURCES:.cpp=.o)

# Цель по умолчанию
all: $(OUTPUT)

# Правило сборки исполняемого файла
$(OUTPUT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Правило для компиляции каждого .cpp в .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка объектных файлов и исполняемого файла
clean:
	rm -f $(OBJECTS) $(OUTPUT)
