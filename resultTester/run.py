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
	definesList = [['RESULT_TEST', define], ['RETURN_ERROR_TEST', define], ['OPTIONAL_TEST', define], ['ERROR_BY_ARGUMENT_TEST', define]]

	sizeOutput = []
	for defines in definesList:
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

def saveSpeedFile(filename, data, iterationsCount):
	with open(filename, 'w') as f:		
		f.write('impl, cycles\n')
		f.write('return, ' + str(data['return'] / iterationsCount) + '\n')
		f.write('argument, ' + str(data['argument'] / iterationsCount) + '\n')
		f.write('optional, ' + str(data['optional'] / iterationsCount) + '\n')
		f.write('result, ' + str(data['result'] / iterationsCount) + '\n')



codeSizeSameUnit = buildConfiguration('Debug', None)
codeSizeSeparateUnit = buildConfiguration('Debug', 'TEST_DIFFERENT_TRANSATION_UNIT')
saveFile('same_size.csv', codeSizeSameUnit)
saveFile('separate_size.csv', codeSizeSeparateUnit)
saveTableFile('size.txt', codeSizeSameUnit, codeSizeSeparateUnit)

#run speed statistics
#buildConfiguration('speedTest', None)
#codeSizeSeparateUnit = buildConfiguration('speedTest', 'TEST_DIFFERENT_TRANSATION_UNIT')
port = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

buildAndFlash('speedTest', ['RESULT_TEST'])
result = port.read(1000)
buildAndFlash('speedTest', ['RETURN_ERROR_TEST'])
returnTest = port.read(1000)
buildAndFlash('speedTest', ['OPTIONAL_TEST'])
optional = port.read(1000)
buildAndFlash('speedTest', ['ERROR_BY_ARGUMENT_TEST'])
argument = port.read(1000)

buildAndFlash('speedTest', ['RESULT_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT'])
resultSeparate = port.read(1000)
buildAndFlash('speedTest', ['RETURN_ERROR_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT'])
returnTestSeparate = port.read(1000)
buildAndFlash('speedTest', ['OPTIONAL_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT'])
optionalSeparate = port.read(1000)
buildAndFlash('speedTest', ['ERROR_BY_ARGUMENT_TEST', 'TEST_DIFFERENT_TRANSATION_UNIT'])
argumentSeparate = port.read(1000)


print result
print returnTest
print optional
print argument

print resultSeparate
print returnTestSeparate
print optionalSeparate
print argumentSeparate

result = result.splitlines()[1]
returnTest = returnTest.splitlines()[1]
optional = optional.splitlines()[1]
argument = argument.splitlines()[1]
sameTranslationUnit = {'result' : int(result), 'return' : int(returnTest), 'optional' : int(optional), 'argument' : int(argument)}
saveSpeedFile('speedSame.csv', sameTranslationUnit, 1000)

resultSeparate = resultSeparate.splitlines()[1]
returnTestSeparate = returnTestSeparate.splitlines()[1]
optionalSeparate = optionalSeparate.splitlines()[1]
argumentSeparate = argumentSeparate.splitlines()[1]
separateTranslationUnit = {'result' : int(resultSeparate), 'return' : int(returnTestSeparate), 'optional' : int(optionalSeparate), 'argument' : int(argumentSeparate)}
saveSpeedFile('speedSeparate.csv', separateTranslationUnit, 1000)

runLatexToPDF(projectDir + '/report.tex')
