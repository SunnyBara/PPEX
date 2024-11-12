bin="myfind"
ref="find"
programme_name="my_find"
nb_tests=0
test_failed=0

#color

red="\033[31m"
green="\033[32m"
yellow="\033[33m"
default="\033[00m"

print_test_category()
{
    echo  "\n${yellow}--------------------------------${default}\n"
    echo  $1
    echo  "\n${yellow}--------------------------------${default}\n"
}

print_ok()
{
    echo  "${green}[OK]${default} ${programme_name} ${1}"
}

# $1 test_name, $2 error

print_ko()
{
    if [ $# -ne 0 ];then
        echo "${red}[KO] ${programme_name} ${1}${default} :\n$2"
    fi
}

print_test_result()
{
    echo "\ntests ran ${yellow}${nb_tests}${default} failed ${red}${test_failed}${default}"
}

clean_up()
{
    rm -f /tmp/{actual,exp}_{err,out}.txt
}


compare_files()
{
    diff -u $2  $3 > /tmp/res_diff.txt 2>&1
    if [ $? -ne 0 ];then
        print_ko "$1" "cat /tmp/res_diff.txt"
        return 1
    fi
    return 0
}

test_case()
{
    nb_tests=$((nb_tests + 1))
    echo "$@"
    find  "$@"  > /tmp/exp_out.txt 2> /tmp/exp_err.txt
    exp_ret=$?
    ./myfind "$@"  > /tmp/actual_out.txt 2> /tmp/actual_err.txt
    actual_ret=$?
    compare_files "$1" /tmp/actual_err.txt /tmp/exp_err.txt
    if [ $? -eq 0 ];then
        compare_files "$1" /tmp/actual_out.txt /tmp/exp_out.txt
        if [ $? -eq 0 ];then
            if [ $exp_ret -eq $actual_ret ];
            then
                print_ok "$@"
                return
            else
                print_ko "$@" "Expected return code ${exp_ret} Actual return code ${actual_ret}"
            fi
        fi
    fi
    test_failed=$((test_failed + 1))
}

test_case_noarg()
{
    nb_tests=$((nb_tests + 1))
    find .  > /tmp/exp_out.txt 2> /tmp/exp_err.txt
    exp_ret=$?
    ./myfind  > /tmp/actual_out.txt 2> /tmp/actual_err.txt
    actual_ret=$?
    compare_files bait /tmp/actual_err.txt /tmp/exp_err.txt
    if [ $? -eq 0 ];then
        compare_files bait /tmp/actual_out.txt /tmp/exp_out.txt
        if [ $? -eq 0 ];then
            if [ $exp_ret -eq $actual_ret ];
            then
                print_ok "no arg"
                return
            else
                print_ko "no arg" "Expected return code ${exp_ret} Actual return code ${actual_ret}"
            fi
        fi
    fi
    test_failed=$((test_failed + 1))
}

test_error_case()
{
    nb_tests=$((nb_tests + 1))
    find $1 > /tmp/exp_out.txt 2> /tmp/exp_err.txt
    exp_ret=$?
    ./myfind $1 > /tmp/actual_out,txt 2> /tmp/actual_err.txt
    actual_ret=$?
    if [ $actual_ret -eq $exp_ret ];
    then
        print_ok "error $1"
        return
    else 
        print_ko "error $1" "Expected return code ${exp_ret} but Got ${actual_ret}"
    fi
    test_failed=$((test_failed + 1))
}

print_test_category "Basic test"

test_case_noarg

test_case .
test_case "src" "src/data"
test_case "src" "-print"
test_case "-name" " 'dat*'"
test_case "-type" "d"
test_case "-newer" "src"
test_case "-user" "sunnysoyer"
test_case "-group" "sunnysoyer"

test_error_case "is_newer ."


print_test_category "Mixing args"

test_case "src" "-name" "'dat*'" "-print"
test_case "src" "-name" "'dat*'" "-print" "-name" "'data.h'" "-print"
test_case "src" "-name" "'dat*'" "-o" "-print"

print_test_result
clean_up

