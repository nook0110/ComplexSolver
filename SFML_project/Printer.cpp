#include "Printer.h"

std::string makeTexture(std::string TeXText, std::string fileName)
{
	std::string filePath = "Textures\\TeX\\";
	std::string filePNG = fileName + "1.png";
	std::string filePathPNG = "Textures\\Equations\\" + filePNG;
	std::string fileTeX = fileName + ".tex";
	std::string filePathTeX = filePath + fileTeX;
	std::ofstream fileTEX(filePathTeX);
	fileTEX << "\\documentclass{article} \n"
		<< "\\usepackage[utf8]{inputenc} \n"
		<< "\\usepackage{amsmath,scalerel} \n"
		<< "\\pagestyle{empty} \n"
		<< "\\usepackage{graphicx} \n"
		<< "\\begin{document} \n";
	fileTEX << "$$" << TeXText << "$$";
	fileTEX << "\n \\end{document}";
	fileTEX.close();
	std::string latexCompile = "latex " + filePathTeX;
	system(latexCompile.c_str());
	std::string fileDVI = fileName + ".dvi";
	std::string filePathDVI = filePath + fileDVI;
	std::string fileAUX = fileName + ".aux";
	std::string filePathAUX = filePath + fileAUX;
	std::string fileLOG = fileName + ".log";
	std::string filePathLOG = filePath + fileLOG;
	rename(fileDVI.c_str(), filePathDVI.c_str());
	rename(fileAUX.c_str(), filePathAUX.c_str());
	rename(fileLOG.c_str(), filePathLOG.c_str());
	std::string dvipngCompile = "dvipng " + filePathDVI + " -D 700 -T tight";
	system(dvipngCompile.c_str());
	rename(filePNG.c_str(), filePathPNG.c_str());
	return filePathPNG;
}

void deleteFiles(std::string fileName)
{
	std::string filePath = "Textures\\TeX\\";
	std::string filePNG = fileName + "1.png";
	std::string filePathPNG = "Textures\\Equations\\" + filePNG;
	std::string fileTeX = fileName + ".tex";
	std::string filePathTeX = filePath + fileTeX;

	std::string fileDVI = fileName + ".dvi";
	std::string filePathDVI = filePath + fileDVI;
	std::string fileAUX = fileName + ".aux";
	std::string filePathAUX = filePath + fileAUX;
	std::string fileLOG = fileName + ".log";
	std::string filePathLOG = filePath + fileLOG;
	system(("del " + filePathPNG).c_str());
	system(("del " + filePathTeX).c_str());
	system(("del " + filePathDVI).c_str());
	system(("del " + filePathLOG).c_str());
	system(("del " + filePathAUX).c_str());
}
