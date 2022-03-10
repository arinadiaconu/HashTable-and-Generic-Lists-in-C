#!/bin/bash

# ----------------- General declarations and util functions ------------------ #

INPUT_DIR=input/
REF_DIR=refs/
OUT_DIR=output/
EXEC_NAME=./tema1
VALGRIND="valgrind --leak-check=full -q --log-file=a.out"
max_points=80
points_per_test=8
points_per_test_vg=2
result_points=0

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

test_do_fail()
{
	printf "failed  [ 0/%02d]\n" "$max_points"
	if test "x$EXIT_IF_FAIL" = "x1"; then
		exit 1
	fi
}

test_do_pass()
{
	printf "passed  [%02d/%02d]\n" "${1}" "$max_points"
	((result_points+=${1}))
}

test_function()
{
	input_value="${1}"
	input_file="$INPUT_DIR${2}"
	output_file="$OUT_DIR${3}"
	ref_file="$REF_DIR${4}"
	
	$EXEC_NAME $input_value $input_file $output_file 

	diff $output_file $ref_file > /dev/null
	if test $? -eq 0; then
		printf "[%s]..........................................." ${2}
		test_do_pass $points_per_test
		#valgrind test

		$VALGRIND $EXEC_NAME $input_value $input_file $output_file > /dev/null
		if [[ -z $(cat a.out) ]]; then
			printf "[VALGRIND: %s]................................." ${2}
			test_do_pass $points_per_test_vg
		else
			printf "[VALGRIND: %s]................................." ${2}
			test_do_fail
		fi

		#rm -rf a.out;
	else
		printf "[%s]..........................................." ${2}
		test_do_fail
		printf "[VALGRIND: %s]................................." ${2}
		test_do_fail
	fi
}

bonus_function()
{
	echo ""
}

init_function()
{
        make build
	mkdir $OUT_DIR &> /dev/null
}

clean_out_function()
{
        rm output/*.*
        rmdir output
}

init_function

print_header "Tema 1 SD HashTable"

#Testing

declare -a input_values=(10 10 19 9 5 4 4 4 1 4)


for i in {0..9}
do
	
	test_function "${input_values[$i]}" "Test$i.in" "Test$i.out" "Test$i.ref"

done

#end Testing

printf "\n%49s [%02d/80]\n" "Total:" $result_points;

clean_out_function

echo 
