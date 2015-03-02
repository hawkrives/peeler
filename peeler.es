import 'babel/polyfill'
import some from 'lodash/collection/some'
import map from 'lodash/collection/map'
import reduce from 'lodash/collection/reduce'
import max from 'lodash/collection/max'
import difference from 'lodash/array/difference'
import take from 'lodash/array/take'
import fs from 'fs'

function getWords(filename='./moby') {
	let file
	try {
		file = fs.readFileSync(filename, {encoding: 'UTF-8'})
	}
	catch (err) {
		console.error(`Error: unable to open ${filename}`)
		process.exit(-1)
	}

	let words = file.split('\n')
	return words
}

function inWordArray(words, str) {
	return some(words, word => word === str)
}

function check(file, dictionary) {
	let query = getWords(file)
	console.log(query.length)
	console.log(dictionary.length)
	console.log(dictionary.length * query.length)

	let start = process.hrtime()

	console.log(take(dictionary, 5), take(query, 5))
	let counter = difference(query, dictionary).length
	// let counter = reduce(
	// 	query,
	// 	(acc, word) => acc + !inWordArray(dictionary, word),
	// 	0)

	console.log(`Misspelled ${counter} words.`)

	let end = process.hrtime(start)
	console.info(`Total cycles: ${end[0] * 1e9 + end[1]}`)
}

function quadrize(str) {
	let len = str.length
	let quadtrant = Math.floor(len / 4)

	if (len < 4) {
		return [str, '', '', '']
	}

	let str1 = str.substr(quadtrant*0, quadtrant)
	let str2 = str.substr(quadtrant*1, quadtrant)
	let str3 = str.substr(quadtrant*2, quadtrant)
	let str4 = str.substr(quadtrant*3, len)

	return [str1, str2, str3, str4]
}

let sum = (a, b) => a + b
let stringToAscii = (str) => map(str, (ch, idx) => str.charCodeAt(idx))
let sumAscii = (str) => reduce(stringToAscii(str), sum, 0)
let strs_to_ascii = (list) => map(list, sumAscii)
let sum_list = (list) => reduce(list, sum, 0)

function seed(max) {
	return [216, 0, 9, 111];
}

function hash_list(list) {
	let m = max(list) + 1
	let seeds = seed(m)

	let hashed = []
	for (let i = 0; i < list.length; i++) {
		hashed[i] = list[i] * seeds[i]
	}
	let sum = sum_list(hashed)
	// console.log(hashed, sum, m)

	return sum % m
}

function main() {
	// let dictionary = getWords('./words')
	// check('./moby', dictionary)

	let test = quadrize("pneumonoultramicroscopicsilicovolcanoconiosis")
	let test2 = quadrize("aa")
	console.log(test)
	console.log(test2)

	let summed = strs_to_ascii(test)
	let summed2 = strs_to_ascii(test2)
	console.log(summed)
	console.log(summed2)

	let hash = hash_list(summed)
	let hash2 = hash_list(summed2)
	console.log(hash)
	console.log(hash2)
}

main()
