# Create build dir if non-existent.
if [ -d "build" ]; then
    echo "Build dir exists"
else
    echo "Creating build folder"
    mkdir "build"
fi

cd build
cmake ..
make all
