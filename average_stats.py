import fileinput
from collections import OrderedDict

# group by the "stats:" heading
# compute for each line in the group, across all groups

firstStatsIndex = -1
secondStatsIndex = -1

input = [line.strip() for line in fileinput.input()]
input = list(filter(lambda line: line, input))

for index, line in enumerate(input):
	if len(line) > 5 and line[:5] == 'stats':
		if firstStatsIndex < 0:
			firstStatsIndex = index
		elif secondStatsIndex < 0:
			secondStatsIndex = index
			break

statsSize = secondStatsIndex - firstStatsIndex

statBlocks = []
for i in range(0, len(input), statsSize):
	statBlocks.append(input[i+1:i+statsSize])

stats = OrderedDict({})
statCounts = {}
for block in statBlocks:
	for line in block:
		description, value = line.split(': ')
		if 'e+19' in value:
			continue
		elif '.' in value:
			value = float(value)
		else:
			value = int(value)
		stats[description] = stats.get(description, 0) + value
		statCounts[description] = statCounts.get(description, 0) + 1

for stat in stats:
	stats[stat] /= statCounts[stat]

for stat, val in stats.items():
	print(stat + ': ', val)
