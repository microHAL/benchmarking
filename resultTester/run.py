from subprocess import call
import subprocess
import sys
import os

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
	return os.popen("arm-none-eabi-size " + project + '/Debug/resultTester.elf').read()

def runLatexToPDF(filename):
	os.system("pdflatex -synctex=1 -interaction=nonstopmode " + filename)


def buildConfiguration(config):
	definesList = [['RESULT_TEST', config], ['RETURN_ERROR_TEST', config], ['ERROR_BY_ARGUMENT_TEST', config]]

	sizeOutput = []
	for defines in definesList:
		sizeOutput.append(runEclipse(projectDir, 'resultTester/Debug', defines))
       
	enabledSize = []
	for i in sizeOutput:
		(text, data, bss, dec, hex , filename) = i.splitlines()[1].split()
		enabledSize.append({'text' : int(text), 'data' : int(data), 'bss' : int(bss), 'dec' : int(dec), 'hex' : hex})
	return enabledSize



def saveFile(filename, data):
	with open(filename, 'w') as f:
		testName = ['result','return','argument']
		f.write('impl,data,bss,text\n')
		for i, size in enumerate(data):
			f.write(testName[i] + ',' + str(size['data']) + ', ' + str(size['bss']) + ', ' + str(size['text']) + '\n')

def saveTableFile(filename, dataFromSameUnit, dataFromSeparateUnit):
	with open(filename, 'w') as f:
		testName = ['result','return','argument']
		f.write('implementation,translation unit,bss,data,text,dec\n')
		for i, size in enumerate(dataFromSameUnit):
			f.write(testName[i] + ',same,' + str(size['bss']) + ', ' + str(size['data']) + ', ' + str(size['text']) + ', ' + str(size['dec']) + '\n')
			f.write(',separate,' + str(dataFromSeparateUnit[i]['bss']) + ', ' + str(dataFromSeparateUnit[i]['data']) + ', ' + str(dataFromSeparateUnit[i]['text']) + ', ' + str(dataFromSeparateUnit[i]['dec']) + '\n')


codeSizeSameUnit = buildConfiguration(None)
codeSizeSeparateUnit = buildConfiguration('TEST_DIFFERENT_TRANSATION_UNIT')
saveFile('same_size.csv', codeSizeSameUnit)
saveFile('separate_size.csv', codeSizeSeparateUnit)
saveTableFile('size.txt', codeSizeSameUnit, codeSizeSeparateUnit)

runLatexToPDF(projectDir + '/report.tex')
