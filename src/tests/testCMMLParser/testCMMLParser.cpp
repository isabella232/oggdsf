// testCMMLParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "libCMMLTags/libCMMLTags.h"

#include <libCMMLParse/libCMMLParse.h>
#include <libCMMLParse/CMMLParser.h>


static int headTestNumber = 0;
static int clipTestNumber = 0;
static int rootTestNumber = 0;

void testHeadParse(wstring inHeadString, bool inShouldPass) {

	CMMLParser locParser;
	C_HeadTag locHead;
	
	bool locWasOK = locParser.parseHeadTag(inHeadString, &locHead);

	headTestNumber++;

	if (locWasOK == inShouldPass) {
		// Either we correctly failed, or correctly passed
	} else if (locWasOK) {
		// We incorrectly passed
		wcout << "*** INCORRECTLY PASSED (Head) ***" << " " << headTestNumber << endl;
		wcout << "Original: " << endl << inHeadString << endl;
		wcout << "Parsed output:" << endl << locHead.toString() << endl << endl;
	} else {
		// We incorrectly failed
		wcout << "*** INCORRECTLY FAILED (Head) ***" << " " << headTestNumber << endl;
		wcout << "Original: " << endl << inHeadString << endl << endl;
	}
}

bool testClipParse(wstring inClipString, bool inShouldPass) {
	CMMLParser locParser;
	C_ClipTag locClip;
	
	bool locWasOK = locParser.parseClipTag(inClipString, &locClip);

	clipTestNumber++;

	if (locWasOK == inShouldPass) {
		// Either we correctly failed, or correctly passed
	} else if (locWasOK) {
		// We incorrectly passed
		wcout << "*** INCORRECTLY PASSED (Clip) ***" << " " << clipTestNumber << endl;
		wcout << "Original: " << endl << inClipString << endl;
		wcout << "Parsed output:" << endl << locClip.toString() << endl << endl;
	} else {
		// We incorrectly failed
		wcout << "*** INCORRECTLY FAILED (Clip) ***" << " " << clipTestNumber << endl;
		wcout << "Original: " << endl << inClipString << endl << endl;
	}

	return locWasOK;
}

bool testCMMLRootParse(wstring inCMMLRootString, bool inShouldPass) {
	CMMLParser locParser;
	C_CMMLRootTag locCMMLRoot;
	
	bool locWasOK = locParser.parseCMMLRootTag(inCMMLRootString, &locCMMLRoot);

	rootTestNumber++;

	if (locWasOK == inShouldPass) {
		// Either we correctly failed, or correctly passed
	} else if (locWasOK) {
		// We incorrectly passed
		wcout << "*** INCORRECTLY PASSED (Root) ***" << " " << rootTestNumber << endl;
		wcout << "Original: " << endl << inCMMLRootString << endl;
		wcout << "Parsed output:" << endl << locCMMLRoot.toString() << endl << endl;
	} else {
		// We incorrectly failed
		wcout << "*** INCORRECTLY FAILED (Root) ***" << " " << rootTestNumber << endl;
		wcout << "Original: " << endl << inCMMLRootString << endl << endl;
	}

	return locWasOK;
}

bool testCMMLFileParse(wstring inFilename) {
	CMMLParser locParser;

	C_CMMLDoc locDoc;
	bool locWasOK = locParser.parseDocFromFile(inFilename, &locDoc);

	wcout<<"Trying to parse "<<inFilename<<endl;

	if (locWasOK) {
		wcout<<locDoc.toString()<<endl;
	} else {
		wcout<<"*** PARSE FAILED ***"<<endl;
	}

	return locWasOK;

}

bool testCMMLClone(wstring inFilename)
{
	CMMLParser locParser;

	C_CMMLDoc locDoc;
	bool locWasOK = locParser.parseDocFromFile(inFilename, &locDoc);

	wcout<<L"Trying to parse "<<inFilename<<endl;

	
	if (locWasOK) {
		wstring locDocStr = locDoc.toString();

		wcout<<endl<<L"Original doc to string..."<<endl;
		wcout<<locDocStr<<endl;

		C_CMMLDoc* clonedDoc = locDoc.clone();

		wcout<<endl<<endl<<L"Cloned version of doc to string"<<endl;

		wstring locCloneDocStr = clonedDoc->toString();

		wcout<<locCloneDocStr<<endl<<endl;

		if (locCloneDocStr == locDocStr) {
			wcout<<L"Output matches... SUCESS"<<endl;

		} else {
			wcout<<L"FAILED CLONE TEST !!"<<endl;

		}
	} else {
		wcout<<"*** PARSE FAILED ***"<<endl;
	}

	return locWasOK;

}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	//Valid minimal
	wstring head_1 = L"<head><title>Types of fish</title><meta name=\"Producer\" content=\"Joe Ordinary\"/><meta name=\"DC.Author\" content=\"Joe's friend\"/></head>";
	testHeadParse(head_1, true);

	//INVALID: Random data
	wstring head_2 = L"asdfasdfasdfasdf";
	testHeadParse(head_2, false);

	//INVALID: Valid xml, invalid cmml
	wstring head_3 = L"<blue><red>random stuff</red><green>But still valid XML</green></blue>";
	testHeadParse(head_3, false);

	//INVALID: No title tag
	wstring head_4 = L"<head><meta name=\"Producer\" content=\"Joe Ordinary\"/><meta name=\"DC.Author\" content=\"Joe's friend\"/></head>";
	testHeadParse(head_4, false);

	//Valid use most
	wstring head_5 = L"<head id=\"headID\" lang=\"en\" dir=\"ltr\" profile=\"some profile\"><base href=\"http://baseurl.com\"/><title lang=\"fr\">Types of fish</title><meta name=\"Producer\" content=\"Joe Ordinary\" scheme=\"some scheme\"/><meta name=\"DC.Author\" content=\"Joe's friend\"/></head>";
	testHeadParse(head_5, true);

	//INVALID: Missing closing xml tag on option element.
	wstring head_6 = L"<head id=\"headID\" lang=\"en\" dir=\"ltr\" profile=\"some profile\"><base href=\"http://baseurl.com\"/><title lang=\"fr\">Types of fish</title><meta name=\"Producer\" content=\"Joe Ordinary\" scheme=\"some scheme\"/><meta name=\"DC.Author\" content=\"Joe's friend\"></head>";
	testHeadParse(head_6, false);


	//Valid minimal
	wstring clip_1 = L"<clip id=\"dolphin\" start=\"npt:3.5\" end=\"npt:5:5.9\"><img src=\"dolphin.jpg\"/><desc>Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip>";
	testClipParse(clip_1, true);

	//INVALID: Random data
	wstring clip_2 = L"asdjhaskdljfhksladf";
	testClipParse(clip_2, false);

	//INVALID: valid xml but invalid cmml
	wstring clip_3 = L"<blue><red>random stuff</red><green>But still valid XML</green></blue>";
	testClipParse(clip_3, false);

	//Valid
	wstring clip_4 = L"<clip id=\"dolphin\" lang=\"en\" start=\"npt:3.5\" end=\"npt:5:5.9\"><img alt=\"Picture of dolphin\" src=\"dolphin.jpg\"/><desc id=\"descID\" lang=\"fr\">Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip>";
	testClipParse(clip_4, true);

	//INVALID: Missing equals on href=
	wstring clip_5 = L"<clip id=\"dolphin\" start=\"npt:3.5\" end=\"npt:5:5.9\"><a href\"http:\\linktome.com\" class=\"someClass\">Random anchor text</a><img src=\"dolphin.jpg\"/><desc>Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip>";
	testClipParse(clip_5, false);

	//Valid
	wstring clip_6 = L"<clip id=\"dolphin\" start=\"npt:3.5\" end=\"npt:5:5.9\"><a href=\"http:\\linktome.com\" class=\"someClass\">Random anchor text</a><img src=\"dolphin.jpg\"/><desc>Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip>";
	testClipParse(clip_6, true);


	//Valid
	wstring cmml_1 = L"<cmml lang=\"en\"><head id=\"headID\" lang=\"en\" dir=\"ltr\" profile=\"some profile\"><base href=\"http://baseurl.com\"/><title lang=\"fr\">Types of fish</title><meta name=\"Producer\" content=\"Joe Ordinary\" scheme=\"some scheme\"/><meta name=\"DC.Author\" content=\"Joe's friend\"/></head><clip id=\"dolphin\" lang=\"en\" start=\"npt:3.5\" end=\"npt:5:5.9\"><img alt=\"Picture of dolphin\" src=\"dolphin.jpg\"/><desc id=\"descID\" lang=\"fr\">Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip></cmml>";
	testCMMLRootParse(cmml_1, true);

	//INVALID: No title
	wstring cmml_2 = L"<cmml lang=\"en\"><head id=\"headID\" lang=\"en\" dir=\"ltr\" profile=\"some profile\"><base href=\"http://baseurl.com\"/><meta name=\"Producer\" content=\"Joe Ordinary\" scheme=\"some scheme\"/><meta name=\"DC.Author\" content=\"Joe's friend\"/></head><clip id=\"dolphin\" lang=\"en\" start=\"npt:3.5\" end=\"npt:5:5.9\"><img alt=\"Picture of dolphin\" src=\"dolphin.jpg\"/><desc id=\"descID\" lang=\"fr\">Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip></cmml>";
	testCMMLRootParse(cmml_2, false);

	//INVALID: No head
	wstring cmml_3 = L"<cmml lang=\"en\"><clip id=\"dolphin\" lang=\"en\" start=\"npt:3.5\" end=\"npt:5:5.9\"><img alt=\"Picture of dolphin\" src=\"dolphin.jpg\"/><desc id=\"descID\" lang=\"fr\">Here, Joe caught sight of a dolphin in the ocean.</desc><meta name=\"Subject\" content=\"dolphin\"/></clip></cmml>";
	testCMMLRootParse(cmml_3, false);

	//INVALID: Random data
	wstring cmml_4 = L"asdfasd fasd fasdf ds ";
	testCMMLRootParse(cmml_4, false);

	//INVALID: valid xml but invalid cmml
	wstring cmml_5 = L"<blue><red>random stuff</red><green>But still valid XML</green></blue>";
	testCMMLRootParse(cmml_5, false);


	wstring file_1 = L"G:\\Media\\Music Vid\\guru.cmml";
	testCMMLFileParse(file_1);


	cout<<"---------------------------------------------------"<<endl;
	wstring file_2 = L"G:\\Media\\Music Vid\\guru.cmml";
	testCMMLClone(file_2);


	return 0;
}

