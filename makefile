BUILD = release
#cosi devo definire tutte le di principali
SOURCE_DIR = src
SOURCEG_DIR= lib/GLAD

BUILD_DIR = build/$(BUILD)
GLAD_DIR= build/GLAD
BINARY = $(BUILD_DIR)/woflan
GCC = g++
LIBS = -lglfw3 -lGLEW -lGLU -lGL -lglut -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lXcursor -lpthread -lrt -lm -ldl   

#######RACCOLTA FILE C E CPP###########
SRCS = $(shell find $(SOURCE_DIR) -name '*.cpp')
SRCSG = $(shell find $(SOURCEG_DIR) -name '*.c')


OBJS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SRCS)) $(patsubst $(SOURCEG_DIR)%.c,$(GLAD_DIR)%.o, $(SRCSG))
DEPS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.d, $(SRCS)) $(patsubst $(SOURCEG_DIR)%.c,$(GLAD_DIR)%.d, $(SRCSG))

all: $(BINARY)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(GCC) $(LIBS) -c $< -o $@
$(GLAD_DIR)/%.o: $(SOURCEG_DIR)/%.c
	$(GCC) $(LIBS) -c $< -o $@

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(GCC) $(LIBS) -MM -MT '$(@:.d=.o)' $< -MF $@
	
$(GLAD_DIR)/%.d: $(SOURCEG_DIR)/%.c
	mkdir -p $(dir $@)
	$(GCC) $(LIBS) -MM -MT '$(@:.d=.o)' $< -MF $@
	
$(BINARY): $(OBJS) $(DEPS)
	$(GCC) $(OBJS) $(LIBS) -o $(BINARY)

static: $(OBJS) $(DEPS)
	$(GCC) $(OBJS) $(LIBS) -static -o $(BINARY)

run: $(BINARY)
	./$(BINARY)

########## Clean

clean-dep:
	rm -f $(DEPS)
clean: clean-dep
	rm -f $(OBJS)

distclean: clean
	rm -fr $(BUILD_DIR)

-include $(DEPS)