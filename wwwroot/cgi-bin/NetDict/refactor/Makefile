FLAGS=-g -std=c++11 -I ~/third_part/include -L ~/third_part/lib -lgflags -lpthread\
			-L /usr/lib64/mysql -lmysqlclient -I ./

main:main.cc index.cc 
	g++ $^ -o $@ $(FLAGS)

.PHONY:clean 
clean:
	rm main
