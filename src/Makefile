CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -I. $(shell pkg-config --cflags Qt6Core Qt6Gui Qt6Widgets)
LDFLAGS = -lgtest -lgtest_main -lpthread $(shell pkg-config --libs Qt6Core Qt6Gui Qt6Widgets)

AR = ar
ARFLAGS = rcs

BUILD_DIR = build
DIST_DIR = dist
GCOV_DIR = gcov_report
DOC = documentation.tex

MODEL_SRC = $(wildcard model/*.cpp)
MODEL_OBJ = $(MODEL_SRC:.cpp=.o)
MODEL_LIB = libmodel.a

TEST_SRC = $(wildcard tests/*.cpp)
QT_DIR = gui/3d
QT_BUILD = $(BUILD_DIR)/3d


all: clean install build_gui

$(MODEL_LIB): $(MODEL_OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(MODEL_LIB)
	@echo "Model library installed."

uninstall:
	rm -rf $(BUILD_DIR) $(DIST_DIR) $(GCOV_DIR) $(MODEL_LIB)
	@echo "Uninstalled build artifacts."

build_gui:
	mkdir -p $(QT_BUILD)
	cd $(QT_BUILD) && qmake6 ../../$(QT_DIR)/3d.pro && make
	mv $(QT_BUILD)/3d $(BUILD_DIR)/3d_app || true
	./$(BUILD_DIR)/3d_app

test: $(MODEL_LIB)
	@echo "Running unit tests..."
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(MODEL_LIB) $(LDFLAGS) -o $(BUILD_DIR)/test_runner
	./$(BUILD_DIR)/test_runner
	rm -f $(BUILD_DIR)/test_runner

gcov_report:
	@echo "Generating coverage report..."
	rm -rf $(GCOV_DIR)
	mkdir -p $(GCOV_DIR)
	$(CXX) $(CXXFLAGS) --coverage $(TEST_SRC) $(MODEL_SRC) $(LDFLAGS) -o $(GCOV_DIR)/gcov_test
	$(GCOV_DIR)/gcov_test || true
	lcov -t "gcov_report" -o $(GCOV_DIR)/coverage.info --no-external --ignore-errors mismatch -c -d .
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/coverage.info
	@echo "Report generated at $(GCOV_DIR)/report/index.html"

dvi:
	@echo "Building documentation..."
	pdflatex $(DOC)
	@echo "Documentation built."

dist: all
	@echo "Creating distribution package..."
	rm -rf $(DIST_DIR)
	mkdir -p $(DIST_DIR)
	cp -r gui model tests Makefile $(DOC) plan.md tasks.md $(DIST_DIR)/
	tar -czf PROJECT.tar.gz $(DIST_DIR)
	mv PROJECT.tar.gz $(DIST_DIR)/
	@echo "Archive created at $(DIST_DIR)/PROJECT.tar.gz"

style:
	@echo "Applying clang-format..."
	clang-format --style=Google -i $(MODEL_SRC) $(TEST_SRC) gui/3d/*.cpp gui/3d/*.h
	@echo "Code style applied."

clean:
	rm -rf $(MODEL_OBJ) $(MODEL_LIB) $(BUILD_DIR) $(DIST_DIR) $(GCOV_DIR) *.o *.log *.aux *.pdf *.out *.toc
	@echo "Clean done."

.PHONY: all install uninstall build_gui test gcov_report dvi dist style clean

