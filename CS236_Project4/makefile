NUM?=4
numbers=1 2 3 5 6 7 8
tests=Lab4PassOffCases

run:
	for number in $(numbers); \
	do \
		./lab$(NUM) $(tests)/4-100/input$$number.txt > $(tests)/out.txt ; \
		diff $(tests)/4-100/answer$$number.txt $(tests)/out.txt || (echo "diff failed on test $$number \n") ; \
	done
	rm $(tests)/out.txt

compile:
	g++ -Wall -Werror -std=c++17 -g *.cpp -o lab$(NUM)
