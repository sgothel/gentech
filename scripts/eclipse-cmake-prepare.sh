#! /bin/sh

sdir=`dirname $(readlink -f $0)`
rootdir=`dirname $sdir`
bname=`basename $0 .sh`

debug_dist_dir="Debug-dist"
debug_build_dir="Debug"
echo debug_dist_dir $debug_dist_dir
echo debug_build_dir $debug_build_dir

release_dist_dir="Release-dist"
release_build_dir="Release"
echo release_dist_dir $release_dist_dir
echo release_build_dir $release_build_dir

cd $rootdir
rm -rf $debug_dist_dir
mkdir -p $debug_dist_dir
rm -rf $debug_build_dir
mkdir -p $debug_build_dir
cd $debug_build_dir
# CLANG_ARGS="-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++"
# CLANG_ARGS="-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_CXX_CLANG_TIDY=/usr/bin/clang-tidy;-p;$rootdir/$debug_build_dir"

#cmake $CLANG_ARGS -DCMAKE_INSTALL_PREFIX=$rootdir/$debug_dist_dir ..
#cmake $CLANG_ARGS -DCMAKE_INSTALL_PREFIX=$rootdir/$debug_dist_dir -DBUILD_TESTING=ON ..
cmake $CLANG_ARGS -DCMAKE_INSTALL_PREFIX=$rootdir/$debug_dist_dir -DDEBUG=ON ..
#cmake $CLANG_ARGS -DCMAKE_INSTALL_PREFIX=$rootdir/$debug_dist_dir -DBUILD_TESTING=ON -DDEBUG=ON ..
#cmake $CLANG_ARGS -DCMAKE_INSTALL_PREFIX=$rootdir/$debug_dist_dir -DBUILD_TESTING=ON -DDEBUG=ON -DINSTRUMENTATION=ON ..

cd $rootdir
rm -rf $release_dist_dir
mkdir -p $release_dist_dir
rm -rf $release_build_dir
mkdir -p $release_build_dir
cd $release_build_dir
# CLANG_ARGS="-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++"
# CLANG_ARGS="-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_CXX_CLANG_TIDY=/usr/bin/clang-tidy;-p;$rootdir/$release_build_dir"

cmake $CLANG_ARGS -DCMAKE_INSTALL_PREFIX=$rootdir/$release_dist_dir ..

cd $rootdir
