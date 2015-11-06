.DEFAULT_GOAL := test

test: 
	g++ -o LoggerTest LoggerTest.cpp CppLog.cpp test_main.cpp -I.. -I. -lboost_unit_test_framework && ./LoggerTest


clean:
	rm LoggerTest
