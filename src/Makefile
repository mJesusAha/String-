FLAGS = -std=c11 -Wall -Werror -Wextra #-fsanitize=address -g
OS := $(shell uname)

ifeq ($(OS),Linux)
	CHFLAGS = -lcheck -lm -lpthread -lrt -lsubunit -D_GNU_SOURCE
else
	CHFLAGS = -lcheck -lm -lpthread
endif


all: clean s21_string.a

s21_string.o:
	gcc $(FLAGS) -c s21_*.c

s21_string.a: s21_string.o
	ar -cr s21_string.a *.o
	rm -rf *.o

test_function.o:
	gcc $(FLAGS) -c test_function.c s21_*.c

test: test_function.c s21_string.a
	gcc $(FLAGS) -fsanitize=address -g test_function.c s21_string.a -o test_function $(CHFLAGS)
#gcc $(FLAGS) -fsanitize=address -g test_function.c s21_string.a -o test_function $(CHFLAGS)
	./test_function

gcov_report: s21_string.a
	gcc --coverage s21_*.c test_function.c -o gcov $(CHFLAGS)
	./gcov
	lcov -t "./gcov" -o report.info --no-external -c -d .
	genhtml -o report report.info
	open ./report/index.html
	rm -rf *.gcno *gcda *.gcov

valgrind: clean
	make s21_string.a
	gcc -nostartfiles $(FLAGS) s21_string.a -o test_valgrind
	CK=FORK=no valgrind --trace-children=yes --track-fsd=yes --track-origins=yes --leak=check=full --show-leak-kinds=all --verbose --log-file=RESULT_VALGRIND.txt ./test_valgrind

style: clean
	clang-format -style=Google -n *.c *.h

clean:
	rm -rf *.o *.a test_function
	rm -rf *.gcno *gcda *.gcov gcov program
	rm -rf report report.info
	rm -rf RESULT_VALGRIND.txt