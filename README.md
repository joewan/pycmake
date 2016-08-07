cmake -Wno-dev
cmake CMakeLists.txt -Wno-dev

brew install graphviz
brew install libtool (/opt/local/lib/libltdl.7.dylib missing)

cmake . --graphviz a

dot -Tgif a -o 1.gif

git clean -d -f -x

http://www.ibm.com/developerworks/cn/aix/library/au-aix-graphviz/

youdao