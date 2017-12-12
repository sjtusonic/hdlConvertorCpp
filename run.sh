#./bin/project.sh
#cd ./build
#make verilog-dot
rm convert
cmake .
make
./convert  > run.log 2>&1

#cd -
#
#cd zjc_run/
# ../build/src/verilog-dot --ast -o out.dot \
# lsu_mon2.v
# 
##simple.v 
#
#cd -
