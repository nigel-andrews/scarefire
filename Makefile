#************************************************
#*                                              *
#*             (c) 2019 J. FABRIZIO             *
#*                                              *
#*                               LRDE EPITA     *
#*                                              *
#************************************************

CC = g++

CPPFLAGS = -Iglm

SRC_DIR = src
OBJ_DIR = build

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CPP_FILES +=
HXX_FILES =
HXX_FILES +=
OBJ_FILES = $(subst $(SRC_DIR),$(OBJ_DIR),$(CPP_FILES:.cpp=.o))

CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++23 -fsanitize=address
CXX_FLAGS +=
CXX_FLAGS += -m64 -march=native
CXX_FLAGS += -fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize
#CXX_FLAGS += -I/home/jonathan/work/enseignement/matieres/synthese/prog/opengl/compute_shader/jf_gl_tools/build_head/src
LDXX_FLAGS = -lGL -lGLU  -lGLEW -lglut -lpthread -lstdc++_libbacktrace -fsanitize=address
#LDXX_FLAGS += /home/jonathan/work/enseignement/matieres/synthese/prog/opengl/compute_shader/jf_gl_tools/dist_lib/libjfglt.a

MAIN_FILE = main.cpp
DIST = main

SKEL_DIST_DIR = pogl_skel_tp
SKEL_FILES = $(CPP_FILES) $(HXX_FILES) $(MAIN_FILE) Makefile vertex.shd fragment.shd


#For gcc 4.9
#CXXFLAGS+=-fdiagnostics-color=auto
export GCC_COLORS=1

define color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then \
	    tput setaf $(1); \
        fi;\
    fi
endef

define default_color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then  tput sgr0 ; fi; \
    fi
endef


all: post-build

pre-build:
	@$(call color,4)
	@echo "******** Starting Compilation ************"
	@if [ ! -d build ]; then mkdir build; fi
	@$(call default_color)

post-build:
	@make --no-print-directory main-build ; \
	sta=$$?;	  \
	$(call color,4); \
	echo "*********** End Compilation **************"; \
	$(call default_color); \
	exit $$sta;

main-build: pre-build build

build: $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(MAIN_FILE) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

build/%.o: src/%.cpp src/%.hh
	@$(call color,2)
	@echo "[$@] $(CXX_FLAGS)"
	@$(call default_color)
	@$(CC) $(CPPFLAGS) -c -o $@ $< $(CXX_FLAGS) ; \
	sta=$$?;	  \
	if [ $$sta -eq 0 ]; then  \
	  $(call color,2) ; \
	  echo "[$@ succes]" ; \
	  $(call default_color) ; \
	else  \
	  $(call color,1) ; \
	  echo "[$@ failure]" ; \
	  $(call default_color) ; \
	fi ;\
	exit $$sta

.PHONY: all clean pre-build post-build main-build build skel

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(DIST)
	rm -rf $(SKEL_DIST_DIR).tar.bz2


skel:
	rm -rf $(SKEL_DIST_DIR)
	mkdir $(SKEL_DIST_DIR)
	cp $(SKEL_FILES) $(SKEL_DIST_DIR)
	tar -cjvf $(SKEL_DIST_DIR).tar.bz2 $(SKEL_DIST_DIR)
	rm -rf $(SKEL_DIST_DIR)
