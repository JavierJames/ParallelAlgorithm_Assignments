CC=g++
#CFLAGS=-c -Wall -fopenmp
CFLAGS=-c -Wall

LFLAGS= 
#LFLAGS= -fopenmp
LDFLAGS=  
INCS= -I
LPATH=
SRCS=pref_suf_c.cpp
OBJS=$(SRCS:%.cpp=%.o)
LIBS=
EXEC=pref_suf_c
CMDL= ./$(EXEC)
RUN_TYPE= RUN_OPENMP 

ifeq ($(RUN_TYPE),RUN_PTHREADS)

	CFLAGS+= -pthread
	LFLAGS+= -lpthread
else 
	
	CFLAGS+= -fopenmp -DRUN_OPENMP -O2 
	LFLAGS+= -fopenmp
	
endif  

all: clean $(SRCS) $(EXEC) run
	echo "run type = $(RUN_TYPE) "
#	$(CC) $(CFLAGS) $(INCS) -c $< -o $@


$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXEC) $(LFLAGS) $(LDFLAGS)
#	$(CC) $(LDFLAGS) $(OBJS) -o $@
#	g++ pref_suf_c.cpp -o pref_suf_c 

run: $(EXEC)
	$(CMDL)

%.o : %.c
#%.o : %.cpp
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@


clean: 
	rm -rf  *o $(EXEC)
