def main():

	x = 55

	y = 99

	count = 0

	while(( count < 10 )): 
		if(( x < y )):
			count++
		
		else:
			return 1
		
	
	return 0

if __name__ == "__main__": 
	import sys
	ret=main()
	sys.exit(ret)

