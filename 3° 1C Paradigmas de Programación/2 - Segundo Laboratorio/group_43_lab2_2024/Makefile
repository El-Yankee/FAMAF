JAVAC = javac
JAVA = java
SRC_DIR = src
LIBS = $(wildcard lib/*.jar)
CLASSPATH = $(SRC_DIR):$(LIBS)
JAVAC_FLAGS = -d bin -cp $(CLASSPATH)
JAVA_FILES = $(shell find $(SRC_DIR) -name "*.java")
CLASS_FILES = $(patsubst $(SRC_DIR)/%.java,bin/%.class,$(JAVA_FILES))
MAIN_CLASS = App
OUTPUT_DIR = bin


all: $(CLASS_FILES)

bin/%.class: src/%.java
	$(JAVAC) $(JAVAC_FLAGS) $<

run: bin/$(MAIN_CLASS).class
	@$(JAVA) -cp $(LIBS):bin $(MAIN_CLASS) $(ARGS)

clean:
	rm -rf $(OUTPUT_DIR)/*.class

.PHONY: all run clean
