cmake -Wno-dev
cmake CMakeLists.txt -Wno-dev

brew install graphviz
brew install libtool (/opt/local/lib/libltdl.7.dylib missing)

cmake . --graphviz a

cmake . --graphviz=moon.viz
dot -Tpng moon.viz -o moon.png

git clean -d -f -x

find . -name *.o -exec rm {} \;

http://www.ibm.com/developerworks/cn/aix/library/au-aix-graphviz/

youdao



tiff compile on mac
brew install dos2unix
dos2unix configure