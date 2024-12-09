# Compiler and flags

# C compiler
CC = gcc

# C compiler flags: enable all warnings and include debug information
CFLAGS = -Wall -g

# C++ compiler
CXX = g++ 

# C++ compiler flags: enable all warnings and include debug information
CXXFLAGS = -Wall -g

# GTest libraries and threading support
GTEST = -lgtest -lgtest_main -lpthread

# Directories

# Directory for API source files
API_DIR = API

# Subdirectory for API source files
API_SRC_DIR = src

# Include path for API headers
API_INCLUDE_DIR = -Iinclude

# Directory for Test Suite source files
TEST_DIR = TestSuite

# Subdirectory for Test Suite source files
TEST_SRC_DIR = src

# Include path for Test Suite headers
TEST_INCLUDE_DIR = -Iinclude

# Directory for build output
BUILD = build

# Directory for report output
REPORT = report

# Directory for XML to XLSX conversion scripts
XLSX = convertXMLtoXLSX

# Directory for XML to HTML conversion scripts
HTML = convertXMLtoHTML

# Detect the operating system
OS := $(shell uname -s 2>/dev/null || echo Windows)

# Define variables for file paths

# Input XML file for conversion
INPUT_FILES = $(XLSX)/ReportTest.xml

# Output XLSX report file
OUTPUT_FILE = $(REPORT)/ReportTest.xlsx

# Output HTML report file
HTML_FILE = $(REPORT)/ReportTest.html

# Rule to create the build directory if it doesn't exist
$(BUILD):
	@mkdir -p $(BUILD)

# Rule to compile .c files from API_DIR into .o files in BUILD
$(BUILD)/%.o: $(API_DIR)/$(API_SRC_DIR)/%.c | $(BUILD)
	@$(CC) $(CFLAGS) $(API_INCLUDE_DIR) -c $< -o $@

# Rule to compile .cc files from TEST_DIR into .o files in BUILD
$(BUILD)/%.o: $(TEST_DIR)/$(TEST_SRC_DIR)/%.cc | $(BUILD)
	@$(CXX) $(CXXFLAGS) $(TEST_INCLUDE_DIR) -c $< -o $@

# Target to build all .o files and link them to create the executable
Build: $(BUILD)/APISrc.o $(BUILD)/TestSuiteSrc.o
	@$(CXX) $(CXXFLAGS) -o $(BUILD)/Report_Program $^ $(GTEST)

# Target to generate an XML report using the built program
xml: Build
	@mkdir -p $(REPORT)
	@echo "Generating XML report in $(REPORT)/ReportTest.xml"
	-@$(BUILD)/Report_Program --gtest_output=xml:$(REPORT)/ReportTest.xml

# Move the XML report to the conversion directory for XLSX processing
moveXML: Build xml
	@mkdir -p $(XLSX)
	@echo "Moving XML report to $(XLSX)/ReportTest.xml"
	@cp $(REPORT)/ReportTest.xml $(XLSX)/ReportTest.xml

# Target to generate a JSON report using the built program
json: Build
	@echo "Generating JSON report in $(REPORT)/ReportTest.json"
	-@$(BUILD)/Report_Program --gtest_output=json:$(REPORT)/ReportTest.json

# Target to convert XML report to HTML
html: Build xml
	@echo "Converting XML report to HTML at $(HTML_FILE)"
	@python3 $(HTML)/xmlTohtml.py $(REPORT)/ReportTest.xml $(HTML_FILE)

# Target to convert XML report to XLSX format
xlsx: Build xml moveXML
	@echo "Generating XLSX report at $(OUTPUT_FILE)"
	@rm -f $(REPORT)/*.xlsx
	@python3 $(XLSX)/xmlToxlsx.py $(INPUT_FILES) --output $(OUTPUT_FILE)

# Target to generate all reports: XML, JSON, HTML, and XLSX
report: Build xml json html xlsx

# Clean up generated files based on the operating system
clean:
ifeq ($(OS), Windows)
	@del /Q $(BUILD)\*.o
	@del /Q $(BUILD)\*.exe
	@del /Q $(REPORT)\*.xml
	@del /Q $(XLSX)\*.xml
	@del /Q $(REPORT)\*.json
	@del /Q $(REPORT)\*.xlsx
	@del /Q $(REPORT)\*.html
else
	@rm -f $(BUILD)/*.o
	@rm -f $(BUILD)/*.exe
	@rm -f $(XLSX)/*.xml
	@rm -f $(REPORT)/*.xlsx
	@rm -f $(REPORT)/*.html
	@rm -f $(REPORT)/*.json
	@rm -f $(REPORT)/*.xml
endif
