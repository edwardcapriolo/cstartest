export CSTAR_TEST=/home/edward/cstartest
export SEA_DIR=/home/edward/seastar
cd $CSTAR_TEST/build
cmake -DCMAKE_PREFIX_PATH="$SEA_DIR/build/release;$SEA_DIR/build/release/_cooking/installed" -DCMAKE_MODULE_PATH=$SEA_DIR/cmake $CSTAR_TEST
make
# run 2 cores 100M memory
#./my_app -c 2  -m 100M
