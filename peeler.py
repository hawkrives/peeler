from functools import reduce

def getWords(filename='./moby'):
	with open(filename, 'r') as file:
		l = []
		for word in file:
			l.append(word)
		return l

def check(file, dictionary):
	query = getWords(file)

	counter = 0
	for word in query:
		if not word in dictionary:
			counter += 1

	print('Misspelled %d words.' % counter)

def main():
	dictionary = getWords('./words')
	check('./moby', dictionary)

if __name__ == '__main__':
	main()
