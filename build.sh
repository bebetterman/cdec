
CMake()
{
	echo cmaking $1 ...
	mkdir $1 -p
	cd $1
	cmake $BUILDHOME/../$1/ -DCMAKE_BUILD_TYPE=$TYPE -DCMAKE_SKIP_BUILD_RPATH=true > cmake.log 2>cmake.msg

	# check cmake result	
	if !(grep "Generating done" cmake.log -i -q)
	then echo in cmake.msg 
		echo cmake error
		echo see ./cmakeBuild$TYPE/$1/cmake.log and cmake.msg
		exit
	fi	

	echo cmake $1 success.	 	
}

Make()
{
	echo making $1 ...
	make > build.log 2>build.msg

	# check build result
	if !(grep "[100%] Built target" build.log -i -q -F)
	then echo in build.msg
		echo build error
		echo see ./cmakeBuild$TYPE/$1/build.log and build.msg
		exit
	fi
	
	echo make $1 success.	
}

Clean()
{
	cd $BUILDHOME
	rm * -r >tmp 2>&1
	rm * -rf		
}

BuildProj()
{
	CMake $1
	Make $1
#	Clean $1
	echo --------------------------------------------------------------------------------------------------
}

MakeDir()
{
	if [ ! -d cmakeBuild$TYPE ]; then
		mkdir cmakeBuild$TYPE
	fi
	cd cmakeBuild$TYPE
	BUILDHOME=$PWD
}

proj=(
	"cdecegg" "cdeccore" "curlcdec" "httpserv" "mysqlcdec" "lzmacdec" "zlibcdec"
	"stavlcommon"
	"TestRunnerC" "testers/TestHttpServer" "testers/TestCurl" "testers/TestCurlCdec"
	"testers/HttpTransport"
)

TYPE=Debug
i=0
len=${#proj[*]}

if [ $2 -a $2 = "Release" ]
then TYPE=Release
	echo Building Release
else
	echo Building Debug
fi

if [ -z $1 ]
then
	echo Usage: ./Build projectname "[Debug|Release]"
	echo default is Debug
   	echo input a project name or "all"
	echo project name are
	while [ $i -lt $len ]
	do
		echo ${proj[$i]}
		let i++
	done
else
	if [ $1 != "all" ]
	then
		while [ $i -lt $len ]
		do
			if [ $1 = ${proj[$i]} ]
			then
				MakeDir
				BuildProj $1
				exit
			fi
			let i++
	   	done
		echo project $1 does not exist!
	else
		MakeDir
		while [ $i -lt $len ]
		do
			echo ${proj[$i]}
			BuildProj ${proj[$i]}
			let i++
		done
	fi	
fi
