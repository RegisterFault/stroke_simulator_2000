stroke: main.c
	cc main.c -o stroke -pthread

clean:
	rm stroke

test: 
	cat LICENSE | ./stroke 
