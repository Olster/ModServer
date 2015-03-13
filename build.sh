# Create build dir if non-existent.
if [ -d "build" ]; then
    echo "build dir exists"
else
    mkdir "build"
fi

cd build
cmake ..
make all

cd ../bin

if [ ! -d "plugins" ]; then
    mkdir plugins
fi

mv *_plugin.so plugins
