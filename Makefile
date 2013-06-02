x86:
	gcc jmpbuster.c -o jmpbuster

arm:
	gcc jmpbustarm.c -o jmpbustarm

clean:
	rm jmpbuster jmpbustarm
