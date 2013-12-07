TYPE=Debug
i=0
len=${#proj[*]}

if [ $1 -a $1 = "Release" ]; then 
  TYPE=Release
fi

./build.sh cdecegg     $TYPE
./build.sh cdeccore    $TYPE
./build.sh zlibcdec    $TYPE
./build.sh lzmacdec    $TYPE
./build.sh mysqlcdec   $TYPE
./build.sh curlcdec    $TYPE
./build.sh httpserv    $TYPE
./build.sh stavlcommon $TYPE
./build.sh TestRunnerC $TYPE
