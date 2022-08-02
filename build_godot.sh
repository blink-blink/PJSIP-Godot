cp SConstruct_b SConstruct
git clone --recursive -b 3.x https://github.com/godotengine/godot-cpp
cd godot-cpp
git submodule update --init --recursive
scons platform=linux target=release
cd ..
scons platform=linux target=release
