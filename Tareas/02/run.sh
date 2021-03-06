#!/bin/bash

if [ "$1" == "--help" ] || [ $# -lt 2 ]; then
	echo -e "Uso: run.sh puerto args_server [valgrind]\n"
	exit -1
fi

server_host=$1
puerto=$2
modo=$4
cant_lientes=500
server="server"
client="client"

if [ "$3" == "valgrind" ]; then
	pre_server='valgrind --gen-suppressions=all --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --log-file=valgrind_server.out '
	pre_client='valgrind --gen-suppressions=all --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --log-file=valgrind_client$n.out '
fi

echo "Probando caso en puerto $1 $2 en modo $4 $3"

shift
shift
shift
shift

echo -n "Server..."
mkfifo server-stdin
$pre_server ./$server $puerto "$@" <> server-stdin > __server_stdout__ 2> __server_stderr__ &
sleep 1
ps $! > /dev/null
server_status=$? 
if [ $! -gt 0 ] && [ $server_status -eq 0 ]; then
	echo -e "OK\nServer PID $!"
	server_pid=$!
else
	echo "Error"
	rm server-stdin
	exit 2
fi


for (( n=0; n < $cant_lientes; n++ )); do
  if [ -f "client$n.in" ]; then
		echo "Cliente $n"
		if [ "$modo" == "single" ]; then
			eval $pre_client ./$client $server_host:$puerto <> client$n.in > client$n.out
		else
			mkfifo input_cliente$n
			eval $pre_client ./$client $server_host:$puerto <> input_cliente$n > client$n.out &
			eval cat client$n.in > input_cliente$n
			rm input_cliente$n
			sleep 2
		fi
	fi
done

if [ "$modo" == "multiple" ]; then
for (( n=0; n < $cant_lientes; n++ )); do
  if [ -f "client$n.in" ]; then
		echo "Cliente $n"
			eval cat client$n.in > input_cliente$n
			sleep 1
		fi
done
sleep 1
for (( n=0; n < $cant_lientes; n++ )); do
  if [ -f "client$n.in" ]; then
		echo "Cliente $n"
			rm input_cliente$n
		fi
done
fi

echo -n "Terminando server..."
echo -e "q\n" >> server-stdin
rm server-stdin
echo "OK"
sleep 1


