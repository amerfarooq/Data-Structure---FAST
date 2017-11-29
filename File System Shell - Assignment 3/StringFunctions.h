#pragma once

#include "NAryTree.h"

string getCurrentDirectory() {
	char directory[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(directory), directory);
	return directory;
}

vector<string> tokenizePath(const string path, const string mainFolder) {
	vector<string> elements;
	fs::path filePath(path);

	bool skip = true;
	for (const auto& p : filePath) {
		if (p.string() == mainFolder) {
			skip = false;
		}
		if (!skip && p.string() != "\\")
			elements.push_back(p.string());
	}
	return elements;
}

string trimPath(string path, string mainFolder) {
	fs::path filePath(path);
	string strApp = "";

	bool skip = true;
	for (const auto& p : filePath) {
		if (p.string() == mainFolder) {
			skip = false;
		}
		if (!skip) {
			strApp.append(p.string() + "\\");
		}
	}
	return strApp.erase(strApp.find_last_of("\\"), 2);
}

bool comparePaths(string mergee, string mergeInto, string mainFolder) {
	vector<string> mergeeTok = tokenizePath(mergee, mainFolder);
	vector<string> mergeIntoTok = tokenizePath(mergeInto, mainFolder);

	string mergeePath = trimPath(mergee, mainFolder);
	string mergeIntoPath = trimPath(mergeInto, mainFolder);

	if (mergeeTok.size() < mergeIntoTok.size()) {
		size_t mergeePathIndex = mergeIntoPath.find(mergeePath);
		size_t mergeIntoPathIndex;

		if (mergeePathIndex != string::npos) {
			mergeIntoPathIndex = mergeIntoPath.find(mergeIntoTok.at(mergeIntoTok.size() - 1));

			if (mergeePathIndex < mergeIntoPathIndex)
				return false;
		}
	}
	return true;
}