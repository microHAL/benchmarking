from subprocess import call
import subprocess
import sys
import os
import serial

projectDir = sys.argv[1]
print projectDir

def runEclipse(project, target, defines):
	workspace = 'workspace_' + project.replace("\\s","_")    
	defs = ''
	for define in defines:
		if define:
	        	defs = defs + ' -D ' + define
	parameters = '--launcher.suppressErrors -nosplash -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + target + '"'
	print parameters
	os.system("eclipse " + parameters)
	return os.popen("arm-none-eabi-size " + project + '/../' + target + '/resultTester.elf').read()

def flash(boardType, binaryfile):
	parameters = '-f board/' + boardType + ' -c init -c targets -c "reset halt" -c "sleep 100" -c "flash write_image erase ' + binaryfile + '" -c "verify_image ' + binaryfile +'" -c "reset run" -c shutdown'
	#print 'openocd ' + parameters
	return os.system("openocd " + parameters)

def runLatexToPDF(filename):
	os.system("pdflatex -synctex=1 -interaction=nonstopmode " + filename)


def buildConfiguration(configuration, define):
	definesList = [['RESULT_TEST'], ['RETURN_ERROR_TEST'], ['OPTIONAL_TEST'], ['ERROR_BY_ARGUMENT_TEST']]

	sizeOutput = []
	for defines in definesList:
		defines = defines + define
		sizeOutput.append(runEclipse(projectDir, 'resultTester/' + configuration, defines))
       
	enabledSize = []
	for i in sizeOutput:
		(text, data, bss, dec, hex , filename) = i.splitlines()[1].split()
		enabledSize.append({'text' : int(text), 'data' : int(data), 'bss' : int(bss), 'dec' : int(dec), 'hex' : hex})
	return enabledSize

def buildAndFlash(configuration, define):
	runEclipse(projectDir, 'resultTester/' + configuration, define)
	flash('st_nucleo_f4.cfg', projectDir + '/' + configuration + '/resultTester.elf')

def saveFile(filename, data):
	with open(filename, 'w') as f:
		testName = ['result', 'return', 'optional', 'argument']
		f.write('impl,data,bss,text\n')
		for i, size in enumerate(data):
			f.write(testName[i] + ',' + str(size['data']) + ', ' + str(size['bss']) + ', ' + str(size['text']) + '\n')

def saveTableFile(filename, dataFromSameUnit, dataFromSeparateUnit):
	with open(filename, 'w') as f:
		testName = ['result', 'return', 'optional', 'argument']
		f.write('implementation,translation unit,bss,data,text,dec\n')
		for i, size in enumerate(dataFromSameUnit):
			f.write(testName[i] + ',same,' + str(size['bss']) + ', ' + str(size['data']) + ', ' + str(size['text']) + ', ' + str(size['dec']) + '\n')
			f.write(',separate,' + str(dataFromSeparateUnit[i]['bss']) + ', ' + str(dataFromSeparateUnit[i]['data']) + ', ' + str(dataFromSeparateUnit[i]['text']) + ', ' + str(dataFromSeparateUnit[i]['dec']) + '\n')

def saveSpeedFile(filename, data):
	with open(filename, 'w') as f:		
		f.write('impl, cycles\n')
		f.write('return, ' + str(data['return']) + '\n')
		f.write('argument, ' + str(data['argument']) + '\n')
		f.write('optional, ' + str(data['optional']) + '\n')
		f.write('result, ' + str(data['result']) + '\n')


def codeSizeStats(settings, filenamePrefix):
	codeSizeSameUnit = buildConfiguration('Debug', [settings])
	codeSizeSeparateUnit = buildConfiguration('Debug', ['TEST_DIFFERENT_TRANSATION_UNIT', settings])
	saveFile(filenamePrefix + '_same_size.csv', codeSizeSameUnit)
	saveFile(filenamePrefix + '_separate_size.csv', codeSizeSeparateUnit)
	saveTableFile(filenamePrefix + '_size.txt', codeSizeSameUnit, codeSizeSeparateUnit)

codeSizeStats('UINT8T_TEST', 'uint8t')
codeSizeStats('UINT32T_TEST', 'uint32t')
codeSizeStats('UINT64T_TEST', 'uint64t')
codeSizeStats('FLAOT_TEST', 'float')

#run speed statistics
#buildConfiguration('speedTest', None)
#codeSizeSeparateUnit = buildConfiguration('speedTest', 'TEST_DIFFERENT_TRANSATION_UNIT')
port = serial.Serial('/dev/ttyACM0', 115200, timeout=2)

def performanceTest(settings, filenamePrefix):
	iterationCount = 100
	buildAndFlash('speedTest', ['RESULT_TEST', settings])
	result = port.read(1000)
	buildAndFlash('speedTest', ['RETURN_ERROR_TEST', settings])
	returnTest = port.read(1000)
	buildAndFlash('speedTest', ['OPTIONAL_TEST', settings])
	optional = port.read(1000)
	buildAndFlash('speedTest', ['ERROR_BY_ARGUMENT_TEST', settings])
	argument = port.read(1000)

	buildAndFlash('speedTest', ['RESULT_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT', settings])
	resultSeparate = port.read(1000)
	buildAndFlash('speedTest', ['RETURN_ERROR_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT', settings])
	returnTestSeparate = port.read(1000)
	buildAndFlash('speedTest', ['OPTIONAL_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT', settings])
	optionalSeparate = port.read(1000)
	buildAndFlash('speedTest', ['ERROR_BY_ARGUMENT_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT', settings])
	argumentSeparate = port.read(1000)

	result = result.splitlines()[1]
	returnTest = returnTest.splitlines()[1]
	optional = optional.splitlines()[1]
	argument = argument.splitlines()[1]
	sameTranslationUnit = {'result' : int(result) / iterationCount, 'return' : int(returnTest) / iterationCount, 'optional' : int(optional) / iterationCount, 'argument' : int(argument) / iterationCount}
	saveSpeedFile(filenamePrefix + '_speedSame.csv', sameTranslationUnit)	

	resultSeparate = resultSeparate.splitlines()[1]
	returnTestSeparate = returnTestSeparate.splitlines()[1]
	optionalSeparate = optionalSeparate.splitlines()[1]
	argumentSeparate = argumentSeparate.splitlines()[1]
	separateTranslationUnit = {'result' : int(resultSeparate) / iterationCount, 'return' : int(returnTestSeparate) / iterationCount, 'optional' : int(optionalSeparate) / iterationCount, 'argument' : int(argumentSeparate) / iterationCount}
	saveSpeedFile(filenamePrefix + '_speedSeparate.csv', separateTranslationUnit)
	
	return [[result, returnTest, optional, argument], [resultSeparate, returnTestSeparate, optionalSeparate, argumentSeparate]]

uint8 = performanceTest('UINT8T_TEST', 'uint8t')
uint32 = performanceTest('UINT32T_TEST', 'uint32t')
uint64 = performanceTest('UINT64T_TEST', 'uint64t')
flo = performanceTest('FLOAT_TEST', 'float')

print uint8[0]
print uint32[0]
print uint64[0]
print flo[0]

print "\n"
print uint8[1]
print uint32[1]
print uint64[1]
print flo[1]

runLatexToPDF(projectDir + '/report.tex')
