# ===========================
#        COMPILER
# ===========================
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# ===========================
#        LIBRARIES
# ===========================
LIBS = -lssl -lcrypto
TEST_LIBS = -lUnitTest++ -lssl -lcrypto

# ===========================
#        SOURCE FILES
# ===========================
SRCS = main.cpp \
       Calculator.cpp \
       ClientCommunicate.cpp \
       Interface.cpp \
       Error.cpp \
       ConnectToBase.cpp

HDRS = Calculator.h \
       ClientCommunicate.h \
       Interface.h \
       Error.h \
       ConnectToBase.h

EXECUTABLE = server

# ===========================
#        TEST FILES
# ===========================
TEST_SRCS = tests/test_main.cpp \
            tests/test_interface.cpp \
            tests/test_calculator.cpp \
            tests/test_connect_to_base.cpp

TEST_EXECUTABLE = tests/run_tests

# ===========================
#        TEST TEMP FILES
# ===========================
TEST_TEMP_FILES = test_db.txt empty_db.txt incomplete_db.txt extra_spaces_db.txt \
                  temp_case_test.txt temp_nonexistent.txt temp_hash_case.txt \
                  temp_auth.txt

# ===========================
#        DEFAULT TARGET
# ===========================
all: $(EXECUTABLE)

# ===========================
#        SERVER BUILD
# ===========================
$(EXECUTABLE): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXECUTABLE) $(LIBS)

# ===========================
#         TEST BUILD
# ===========================
tests: $(TEST_SRCS) $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) \
		Calculator.cpp ConnectToBase.cpp Interface.cpp Error.cpp ClientCommunicate.cpp \
		-o $(TEST_EXECUTABLE) $(TEST_LIBS)

# ===========================
#        RUN TESTS
# ===========================
run-tests: tests
	./$(TEST_EXECUTABLE)

# ===========================
#          CLEAN
# ===========================
clean:
	rm -f $(EXECUTABLE)
	rm -f $(TEST_EXECUTABLE)
	rm -f $(TEST_TEMP_FILES)
	rm -f *.txt~ *.cpp~ *.h~
	rm -f *.log *.out
	rm log.txt

# ===========================
#        CLEAN TESTS ONLY
# ===========================
clean-tests:
	rm -f $(TEST_EXECUTABLE)
	rm -f $(TEST_TEMP_FILES)

# ===========================
#        FULL CLEAN
# ===========================
distclean: clean
	rm -f *~
	rm -rf *.dSYM

.PHONY: all clean clean-tests distclean tests run-tests