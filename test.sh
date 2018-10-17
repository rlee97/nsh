echo -----TEST OUTPUT WILL BE DISPLAYED-----
echo -----PLEASE CHECK test.sh FOR COMMANDS-----

echo ________________________________TESTING 1 PIPE_________________________________
echo

echo ---test 1--- ls and sort
ls | sort

echo ---test 2--- ls -l and grep parse
ls -l | grep parse
echo

echo _________________________________TESTING cd_____________________________________
echo

echo ---test1--- pwd
pwd
echo ---test2--- mkdir test
mkdir test
ls
echo ---test3--- cd into test and pwd
cd test
pwd
echo ---test4--- cd ..
cd ..
ls
echo ---test5--- pwd
pwd
rm -r test
echo

echo ________________TESTING input redirection and output redirection________________
echo

echo ---test1--- CREATE test.txt FILE WITH "TESTING INPUT" AND CAT
echo TESTING INPUT > test.txt
cat test.txt

echo ---test2--- APPEND "MORE INPUT" AND CAT
echo MORE INPUT >> test.txt
cat test.txt

echo ---test3--- INPUT REDIRECT TO CAT
cat < test.txt

rm test.txt
echo

echo ______________________________TESTING multiple pipes_____________________________
echo

echo ---test1--- TESTING 10 CAT PIPES
echo hello | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat

echo ---test2--- TESTING ls -l, sort, and grep
ls -l | sort | grep parse.c
echo

echo ____________________TESTING multiples pipes with redirection_____________________
echo

echo ---test1--- PRINTS 2 OF THE SAME LINE TESTING 10 CAT PIPES WITH 1 INPUT AND 1 OUTPUT REDIRECT

echo Hello grader, have a nice summer! > message.txt
cat < message.txt | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat >> message.txt
cat message.txt
rm message.txt


