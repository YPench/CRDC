/*
 * Copyright (C) 2015 by Yanping Chen <ypench@gmail.com>
 * Begin       : 01-Oct-2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser GPL (LGPL) as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 */

#include "stdafx.h"
#include "SXMLer.h"
#include "..\\Include\\ace.h"
//#include "..\\Include\\CGCom.h"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <iostream>

using namespace xercesc;


SXMLer::SXMLer()
{
	Xercesc_Initialize_Flag = false;
}
SXMLer::~SXMLer()
{
	if(Xercesc_Initialize_Flag){
		XMLPlatformUtils::Terminate();
	}
	
}

bool SXMLer::Xercesc_ACE_Action_Mention_Extractor(const char* xmlFile, ACE_Corpus& m_ACE_Corpus)
{
	string path = xmlFile;
	vector<string> FilePath_v;
	if(!NLPOP::Exist_of_This_File(path + "apf.v5.1.1.dtd")){
		AppCall::Secretary_Message_Box("The pointed .dtd file is not exist...", MB_OK);
		return "";
	}
	NLPOP::Get_Specified_Files(xmlFile, FilePath_v, ".xml");

	if(!Xercesc_Initialize_Flag){
			Xercesc_Initialize();
	}
	map<string, ACE_Event>& ACE_EventInfo_m = m_ACE_Corpus.ACE_EventInfo_m;
	for(size_t i = 0; i < FilePath_v.size(); i++){
		string apfname = FilePath_v[i].substr(FilePath_v[i].rfind('.'), FilePath_v[i].length());
		if(strcmp(apfname.c_str(), ".xml")){
			continue;
		}
		apfname = FilePath_v[i].substr(0, FilePath_v[i].rfind('.'));
		apfname  = apfname.substr(apfname.rfind('.'), apfname.length());
		if(strcmp(apfname.c_str(), ".apf")){
			continue;
		}
		apfname = FilePath_v[i];

		XercesDOMParser* parser = new XercesDOMParser();
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);    // optional

		ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
		parser->setErrorHandler(errHandler);
		try {
			parser->parse(apfname.c_str());
		 }
		catch (...) {
			cout << "Unexpected Exception \n" ;
			return false;
		}
		xercesc_3_1::DOMDocument* xmlDoc = parser->getDocument();
		DOMElement * Xroot = xmlDoc->getDocumentElement();
		DOMElement * pDocElement;
		DOMElement * pACE_Events;

		if(Xroot){
			if(!XMLString::compareIString(Xroot->getTagName(), XMLString::transcode("source_file"))){
				string URI = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("URI")));
				string SOURCE = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("SOURCE")));
				string TYPE = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("TYPE")));
				string AUTHOR = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("AUTHOR")));
				string ENCODING = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("ENCODING")));
				pDocElement = Xroot->getFirstElementChild();
				if(!XMLString::compareIString(pDocElement->getTagName(), XMLString::transcode("document"))){
					string DOCID = XMLString::transcode(pDocElement->getAttribute(XMLString::transcode("DOCID")));

					pACE_Events = pDocElement->getFirstElementChild();
					while(pACE_Events){
						if(!XMLString::compareIString(pACE_Events->getTagName(), XMLString::transcode("event"))){
							break;
						}
						pACE_Events = pACE_Events->getNextElementSibling();
					}
					if(pACE_Events){
						while(!XMLString::compareIString(pACE_Events->getTagName(), XMLString::transcode("event"))){
							XMLSize_t eventcnt = pACE_Events->getChildElementCount();;
							ACE_Event loc_event;
							loc_event.DOCID = DOCID;
							loc_event.ID = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("ID")));
							loc_event.TYPE = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("TYPE")));
							loc_event.SUBTYPE = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("SUBTYPE")));
							loc_event.MODALITY = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("MODALITY")));
							loc_event.POLARITY = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("POLARITY")));
							loc_event.GENERICITY = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("GENERICITY")));
							loc_event.TENSE = XMLString::transcode(pACE_Events->getAttribute(XMLString::transcode("TENSE")));
							DOMElement * pEvent = pACE_Events->getFirstElementChild();
							for(XMLSize_t i = 0; i < eventcnt; i++){
								if(!XMLString::compareIString(pEvent->getTagName(), XMLString::transcode("event_argument"))){
									loc_event.event_argument_v.push_back(make_pair(XMLString::transcode(pEvent->getAttribute(XMLString::transcode("REFID")))
										, XMLString::transcode(pEvent->getAttribute(XMLString::transcode("ROLE")))));
								}
								else if(!XMLString::compareIString(pEvent->getTagName(), XMLString::transcode("event_mention"))){
									event_mention loc_mention;
									loc_mention.ID = XMLString::transcode(pEvent->getAttribute(XMLString::transcode("ID")));
									XMLSize_t mentioncnt = pEvent->getChildElementCount();
									DOMElement * pnode = pEvent->getFirstElementChild();
									for(XMLSize_t j = 0; j < mentioncnt; j++){
										if(!XMLString::compareIString(pnode->getTagName(), XMLString::transcode("extent"))){
											DOMElement * pextend = pnode->getFirstElementChild();
											loc_mention.extent.START = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("START"))));
											loc_mention.extent.END = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("END"))));
											loc_mention.extent.charseq = XMLString::transcode(pextend->getTextContent());
										}
										else if(!XMLString::compareIString(pnode->getTagName(), XMLString::transcode("ldc_scope"))){
											DOMElement * pextend = pnode->getFirstElementChild();
											loc_mention.ldc_scope.START = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("START"))));
											loc_mention.ldc_scope.END = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("END"))));
											loc_mention.ldc_scope.charseq = XMLString::transcode(pextend->getTextContent());
										}
										else if(!XMLString::compareIString(pnode->getTagName(), XMLString::transcode("anchor"))){
											DOMElement * pextend = pnode->getFirstElementChild();
											loc_mention.anchor.START = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("START"))));
											loc_mention.anchor.END = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("END"))));
											loc_mention.anchor.charseq = XMLString::transcode(pextend->getTextContent());
										}
										else if(!XMLString::compareIString(pnode->getTagName(), XMLString::transcode("event_mention_argument"))){
											event_mention_argument loc_argment;
											loc_argment.REFID = XMLString::transcode(pnode->getAttribute(XMLString::transcode("REFID")));
											loc_argment.ROLE = XMLString::transcode(pnode->getAttribute(XMLString::transcode("ROLE")));

											DOMElement * pextend = pnode->getFirstElementChild();
											pextend = pextend->getFirstElementChild();
											loc_argment.extent.START = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("START"))));
											loc_argment.extent.END = atoi(XMLString::transcode(pextend->getAttribute(XMLString::transcode("END"))));
											loc_argment.extent.charseq = XMLString::transcode(pextend->getTextContent());
											
											loc_mention.event_mention_argument_v.push_back(loc_argment);
										}
										pnode = pnode->getNextElementSibling();
									}
									loc_event.event_mention_v.push_back(loc_mention);
								}
								pEvent = pEvent->getNextElementSibling();
							}
							ACE_EventInfo_m.insert(make_pair(loc_event.ID, loc_event));
							pACE_Events = pACE_Events->getNextElementSibling();
							if(!pACE_Events){
								break;
							}
						}
					}
				}
			}
		}
		delete parser;
		delete errHandler;
	}
	return true;
}

string SXMLer::Xercesc_ACE_Folder_For_sgm_Files_Filter(string path, ACE_Corpus& m_ACE_Corpus)
{
	string filename;
	vector<string> FilePath_v;
	NLPOP::Get_Specified_Files(path, FilePath_v, ".sgm");
	for(unsigned long i = 0; i < FilePath_v.size(); i++){
		Xercesc_ACE_sgm_Content_Extractor(FilePath_v[i].c_str(), m_ACE_Corpus);
	}
	return "";
}
string SXMLer::Xercesc_ACE_Folder_Files_Filter(string path, ACE_Corpus& m_ACE_Corpus)
{
	string filename;
	vector<string> FilePath_v;
	if(!NLPOP::Exist_of_This_File(path + "apf.v5.1.1.dtd")){
		AppCall::Secretary_Message_Box("The pointed .dtd file is not exist...", MB_OK);
		return "";
	}
	NLPOP::Get_Specified_Files(path, FilePath_v, "");
	for(unsigned long i = 0; i < FilePath_v.size(); i++){
		if(FilePath_v[i].rfind('.') == -1){
			continue;
		}
		filename = FilePath_v[i].substr(FilePath_v[i].rfind('.'), FilePath_v[i].length());
		if(strcmp(filename.c_str(), ".xml")){
			continue;
		}
		filename = FilePath_v[i].substr(0, FilePath_v[i].rfind('.'));
		filename = filename.substr(filename.rfind('.'), filename.length());
		if(strcmp(filename.c_str(), ".apf")){
			continue;
		}
		Xercesc_ACE_Relation_Corpus_Extractor(FilePath_v[i].c_str(), m_ACE_Corpus);
	}
	return "";
}


bool SXMLer::Xercesc_ACE_sgm_Content_Extractor(const char* xmlFile, ACE_Corpus& m_ACE_Corpus)
{
	map<string, ACE_sgm>& ACE_sgm_mmap = m_ACE_Corpus.ACE_sgm_mmap;
	
	if(!Xercesc_Initialize_Flag){
			Xercesc_Initialize();
	}
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);
	try {
		parser->parse(xmlFile);
     }
	catch (...) {
		cout << "Unexpected Exception \n" ;
		//AppCall::Secretary_Message_Box("Unexpected Exception...", MB_OK);
		return false;
	}
	string outstr;
	string s_DOCID;
	xercesc_3_1::DOMDocument* xmlDoc = parser->getDocument();
	DOMNodeList* doc_DodeList;

	DOMElement * Xroot = xmlDoc->getDocumentElement();

	doc_DodeList = Xroot->getElementsByTagName(XMLString::transcode("DOCID"));
	if(doc_DodeList->getLength() != 1){
		outstr = "<DOCID> != 1: ";
		outstr += xmlFile;
		AppCall::Secretary_Message_Box(outstr, MB_OK);
	}

	s_DOCID = XMLString::transcode(doc_DodeList->item(0)->getTextContent());
	if(s_DOCID.c_str()[0] == ' '){
		s_DOCID = s_DOCID.substr(1, s_DOCID.length()-1);
	}
	if(s_DOCID.c_str()[s_DOCID.length() - 1] == ' '){
		s_DOCID = s_DOCID.substr(0, s_DOCID.length()-1);
	}
	ACE_sgm_mmap[s_DOCID];
	ACE_sgm_mmap[s_DOCID].DOCID = s_DOCID;

	doc_DodeList = Xroot->getElementsByTagName(XMLString::transcode("DOCTYPE"));
	if(doc_DodeList->getLength() != 1){
		outstr = "<DOCTYPE>: ";
		outstr += xmlFile;
		AppCall::Secretary_Message_Box(outstr, MB_OK);
	}
	ACE_sgm_mmap[s_DOCID].DOCTYPE =XMLString::transcode(doc_DodeList->item(0)->getTextContent());

	doc_DodeList = Xroot->getElementsByTagName(XMLString::transcode("DATETIME"));
	if(doc_DodeList->getLength() != 1){
		outstr = "<DATETIME>: ";
		outstr += xmlFile;
		AppCall::Secretary_Message_Box(outstr, MB_OK);
	}
	ACE_sgm_mmap[s_DOCID].DATETIME =XMLString::transcode(doc_DodeList->item(0)->getTextContent());

	//------------------------------------------
	doc_DodeList = Xroot->getElementsByTagName(XMLString::transcode("TEXT"));
	if(doc_DodeList->getLength() != 1){
		outstr = "<TEXT>: ";
		outstr += xmlFile;
		AppCall::Secretary_Message_Box(outstr, MB_OK);
	}
	string TEXTstr =XMLString::transcode(doc_DodeList->item(0)->getTextContent());

	if(Xroot){
		if(!XMLString::compareIString(Xroot->getTagName(), XMLString::transcode("DOC"))){
			ACE_sgm_mmap[s_DOCID].DOC =XMLString::transcode(Xroot->getTextContent());
			ACE_sgm_mmap[s_DOCID].TEXT = TEXTstr;
			if(ACE_sgm_mmap[s_DOCID].DOC.find(TEXTstr.c_str()) == string::npos){
				AppCall::Secretary_Message_Box("TEXT substring position is wrong...", MB_OK);
			}
		}
		else{
			AppCall::Secretary_Message_Box("Not equal with DOC...", MB_OK);
		}
	}
	else{
		AppCall::Secretary_Message_Box("Xroot is failed...", MB_OK);
	}
	delete parser;
	delete errHandler;
	return false;
}



bool SXMLer::Xercesc_Gigaword_Content_Extractor(const char* xmlFile, vector<pCGigaDOC>& m_GigaDOC_v, const char* TYPE)
{

	vector<string> FilePath_v;
	string path = xmlFile;
	//if(!NLPOP::Exist_of_This_File(path + "gigaword_c.dtd")){
	//	AppCall::Secretary_Message_Box("The pointed .dtd file is not exist...", MB_OK);
	//	return "";
	//}
	NLPOP::Get_Specified_Files(path, FilePath_v, TYPE);
	if(!Xercesc_Initialize_Flag){
			Xercesc_Initialize();
	}

	m_GigaDOC_v.reserve(200000);
	DOMElement * Xroot;
	DOMElement* pDocElement;
	DOMElement* pElement;
	for(size_t i = 0; i < FilePath_v.size(); i++){
		//path = FilePath_v[i].substr(FilePath_v[i].rfind('\\')+1, FilePath_v[i].length()).c_str();
		//if(!strcmp(path.c_str(), "gigaword_c.dtd")){
		//	continue;
		//}
		XercesDOMParser* parser = new XercesDOMParser();
		ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);    // optional
		parser->setErrorHandler(errHandler);
		try {
			parser->parse(FilePath_v[i].c_str());
		 }
		catch (...) {
			cout << "Unexpected Exception \n" ;
			AppCall::Secretary_Message_Box("Unexpected Exception...", MB_OK);
			delete parser;
			delete errHandler;
			continue;
		}
		xercesc_3_1::DOMDocument* xmlDoc = parser->getDocument();;
		
		Xroot = xmlDoc->getDocumentElement();
		XMLSize_t doccnt = Xroot->getChildElementCount();
		pDocElement = Xroot->getFirstElementChild();
		pDocElement = Xroot;
		for(XMLSize_t j = 0; j < doccnt; j++){
			pCGigaDOC plocGigaDOC = new CGigaDOC;
			plocGigaDOC->p_DOCID = NULL;
			plocGigaDOC->p_TYPE = NULL;
			plocGigaDOC->p_Headline = NULL;
			plocGigaDOC->p_Dataline = NULL;
			plocGigaDOC->p_TEXT = NULL;
			if(XMLString::compareIString(pDocElement->getTagName(), XMLString::transcode("DOC"))){
				AppCall::Secretary_Message_Box("Unmatched Data in: SXMLer::Xercesc_Gigaword_Content_Extractor()");
			}
			plocGigaDOC->p_DOCID = XMLString::transcode(pDocElement->getAttribute(XMLString::transcode("id")));
			plocGigaDOC->p_TYPE = XMLString::transcode(pDocElement->getAttribute(XMLString::transcode("type")));
			
			XMLSize_t elementcnt = pDocElement->getChildElementCount();
			pElement = pDocElement->getFirstElementChild();
			for(XMLSize_t k = 0; k < elementcnt; k++){
				if(!XMLString::compareIString(pElement->getTagName(), XMLString::transcode("HEADLINE"))){
					plocGigaDOC->p_Headline = XMLString::transcode(pElement->getTextContent());
				}
				else if(!XMLString::compareIString(pElement->getTagName(), XMLString::transcode("DATELINE"))){
					plocGigaDOC->p_Dataline = XMLString::transcode(pElement->getTextContent());
				}
				else if(!XMLString::compareIString(pElement->getTagName(), XMLString::transcode("TEXT"))){
					plocGigaDOC->p_TEXT = XMLString::transcode(pElement->getTextContent());
				}
				pElement = pElement->getNextElementSibling();
			}
			m_GigaDOC_v.push_back(plocGigaDOC);
			pDocElement = pDocElement->getNextElementSibling();
		}
		delete parser;
		delete errHandler;
	}
	return 0;
}




bool SXMLer::Xercesc_ACE_Relation_Corpus_Extractor(const char* xmlFile, ACE_Corpus& m_ACE_Corpus)
{
	vector<ACE_relation>& ACE_Relation_Info_v = m_ACE_Corpus.ACE_Relation_Info_v;
	map<string, ACE_entity>& ACE_Entity_Info_map = m_ACE_Corpus.ACE_Entity_Info_map;

	if(!Xercesc_Initialize_Flag){
			Xercesc_Initialize();
	}
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);
	try {
		parser->parse(xmlFile);
     }
     catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n"
			<< message << "\n";
		XMLString::release(&message);
		return false;
	}
	catch (const DOMException& toCatch) {
		char* message = XMLString::transcode(toCatch.msg);
		cout << "Exception message is: \n"
                 << message << "\n";
		XMLString::release(&message);
		return false;
	}
	catch (const SAXException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n"
			<< message << "\n";
		XMLString::release(&message);
		return false;
	}
	catch (...) {
		cout << "Unexpected Exception \n" ;
		return false;
	}
	xercesc_3_1::DOMDocument* xmlDoc = parser->getDocument();
	
	DOMElement* documentElement;
	DOMElement* ACE_Element;
	DOMElement* entity_child_Element;
	DOMElement* entity_mention_child_Element;
	DOMElement* entity;
	DOMElement* entity_mention;
	DOMElement* entity_attributes;
	DOMElement* relation;
	DOMElement* relation_child_Element;
	DOMElement* relation_argument;
	DOMElement* relation_mention;
	DOMElement* relation_mention_argument;  
	DOMElement* extent;
	DOMElement* head;
	DOMElement* name;
	DOMElement* charseq;

	DOMElement * Xroot = xmlDoc->getDocumentElement();
	//const XMLCh* viewerch;
	istringstream sstream;
	string Arg_Role;
	string sDOCID;
	if(Xroot){
		if(!XMLString::compareIString(Xroot->getTagName(), XMLString::transcode("source_file"))){
			documentElement = Xroot->getFirstElementChild();
			if(!XMLString::compareIString(documentElement->getTagName(), XMLString::transcode("document"))){
				sDOCID = XMLString::transcode(documentElement->getAttribute(XMLString::transcode("DOCID")));
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID];
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID].DOCID = sDOCID;
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID].URI = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("URI")));
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID].SOURCE = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("SOURCE")));
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID].TYPE = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("TYPE")));
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID].AUTHOR = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("AUTHOR")));
				m_ACE_Corpus.ACE_DocInfo_m[sDOCID].ENCODING = XMLString::transcode(Xroot->getAttribute(XMLString::transcode("ENCODING")));

				ACE_Element = documentElement->getFirstElementChild();
				while(ACE_Element){
					if(!XMLString::compareIString(ACE_Element->getTagName(), XMLString::transcode("entity"))){
						entity = ACE_Element;
						ACE_entity m_ACE_entity;
						m_ACE_entity.DOCID = sDOCID;
						m_ACE_entity.ID = XMLString::transcode(entity->getAttribute(XMLString::transcode("ID")));
						m_ACE_entity.TYPE = XMLString::transcode(entity->getAttribute(XMLString::transcode("TYPE")));
						m_ACE_entity.SUBTYPE = XMLString::transcode(entity->getAttribute(XMLString::transcode("SUBTYPE")));
						m_ACE_entity.CLASS = XMLString::transcode(entity->getAttribute(XMLString::transcode("CLASS")));
						entity_child_Element = entity->getFirstElementChild();
						while(entity_child_Element){
							if(!XMLString::compareIString(entity_child_Element->getTagName(), XMLString::transcode("entity_mention"))){
								entity_mention = entity_child_Element;
								ACE_entity_mention m_ACE_entity_mention;
								//-----additional info
								m_ACE_entity_mention.DOCID = sDOCID;
								m_ACE_entity_mention.Entity_ID = m_ACE_entity.ID;
								m_ACE_entity_mention.Entity_TYPE = m_ACE_entity.TYPE;
								m_ACE_entity_mention.Entity_SUBSTYPE = m_ACE_entity.SUBTYPE;
								m_ACE_entity_mention.Entity_CLASS = m_ACE_entity.CLASS;
								//</additional info>
								m_ACE_entity_mention.ID = XMLString::transcode(entity_mention->getAttribute(XMLString::transcode("ID")));
								m_ACE_entity_mention.TYPE = XMLString::transcode(entity_mention->getAttribute(XMLString::transcode("TYPE")));
								m_ACE_entity_mention.LDCTYPE = XMLString::transcode(entity_mention->getAttribute(XMLString::transcode("LDCTYPE")));
								m_ACE_entity_mention.LDCATR = XMLString::transcode(entity_mention->getAttribute(XMLString::transcode("LDCATR")));
								entity_mention_child_Element = entity_mention->getFirstElementChild();
								while(entity_mention_child_Element){
									if(!XMLString::compareIString(entity_mention_child_Element->getTagName(), XMLString::transcode("extent"))){
										extent = entity_mention_child_Element;
										charseq = extent->getFirstElementChild();
										if(charseq){
											sstream.clear();
											sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("START"))));
											sstream >> m_ACE_entity_mention.extent.START;
											sstream.clear();
											sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("END"))));
											sstream >> m_ACE_entity_mention.extent.END;
										}
										m_ACE_entity_mention.extent.charseq = XMLString::transcode(charseq->getTextContent());
									}
									if(!XMLString::compareIString(entity_mention_child_Element->getTagName(), XMLString::transcode("head"))){
										head = entity_mention_child_Element;
										charseq = head->getFirstElementChild();
										if(charseq){
											sstream.clear();
											sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("START"))));
											sstream >> m_ACE_entity_mention.head.START;
											sstream.clear();
											sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("END"))));
											sstream >> m_ACE_entity_mention.head.END;
										}
										m_ACE_entity_mention.head.charseq = XMLString::transcode(charseq->getTextContent());
									}//head
									entity_mention_child_Element = entity_mention_child_Element->getNextElementSibling();
								}//while(entity_mention_child_Element)
/*input mention*/				m_ACE_entity.entity_mention_v.push_back(m_ACE_entity_mention);
							}//if entity_mention
							else if(!XMLString::compareIString(entity_child_Element->getTagName(), XMLString::transcode("entity_attributes"))){
								entity_attributes = entity_child_Element;
								name = entity_attributes->getFirstElementChild();
								while(name){
									ACE_name m_ACE_name;
									charseq = name->getFirstElementChild();
									if(charseq){
										sstream.clear();
										sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("START"))));
										sstream >> m_ACE_name.START;
										sstream.clear();
										sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("END"))));
										sstream >> m_ACE_name.END;
									}
									m_ACE_name.charseq = XMLString::transcode(charseq->getTextContent());
/*input attribute*/					m_ACE_entity.entity_attributes.name_v.push_back(m_ACE_name);
									name = name->getNextElementSibling();
								}
							}//if attribute
							entity_child_Element = entity_child_Element->getNextElementSibling();
						}//while(entity_child_Element){
						/*input entity*/
						if(ACE_Entity_Info_map.find(m_ACE_entity.ID) != ACE_Entity_Info_map.end()){
							AppCall::Secretary_Message_Box("重复的命名实体，在函数：SXercesc::Xercesc_ACE_Relation_Corpus_Extractor()",MB_OK);
						}
						else{
							ACE_Entity_Info_map.insert(make_pair(m_ACE_entity.ID, m_ACE_entity));
						}
					}// entity
					else if(!XMLString::compareIString(ACE_Element->getTagName(), XMLString::transcode("relation"))){
						relation = ACE_Element;
						ACE_relation m_ACE_relation;//***********
						m_ACE_relation.DOCID = sDOCID;
						m_ACE_relation.ID = XMLString::transcode(relation->getAttribute(XMLString::transcode("ID")));
						m_ACE_relation.TYPE	= XMLString::transcode(relation->getAttribute(XMLString::transcode("TYPE")));
						m_ACE_relation.SUBTYPE = XMLString::transcode(relation->getAttribute(XMLString::transcode("SUBTYPE")));
						m_ACE_relation.TENSE	= XMLString::transcode(relation->getAttribute(XMLString::transcode("TENSE")));	
						m_ACE_relation.MODALITY= XMLString::transcode(relation->getAttribute(XMLString::transcode("MODALITY")));
						relation_child_Element = relation->getFirstElementChild();
						while(relation_child_Element){
							if(!XMLString::compareIString(relation_child_Element->getTagName(), XMLString::transcode("relation_argument"))){
								relation_argument = relation_child_Element;
								ACE_relation_argument m_ACE_relation_argument;
								m_ACE_relation_argument.REFID = XMLString::transcode(relation_argument->getAttribute(XMLString::transcode("REFID")));
								m_ACE_relation_argument.ROLE = XMLString::transcode(relation_argument->getAttribute(XMLString::transcode("ROLE")));
								m_ACE_relation.relation_argument_v.push_back(m_ACE_relation_argument);
							}
							else if(!XMLString::compareIString(relation_child_Element->getTagName(), XMLString::transcode("relation_mention"))){
									relation_mention = relation_child_Element;
									ACE_relation_mention m_ACE_relation_mention;//***********
									m_ACE_relation_mention.DOCID = sDOCID;
									m_ACE_relation_mention.ID = XMLString::transcode(relation_mention->getAttribute(XMLString::transcode("ID")));
									m_ACE_relation_mention.LEXICALCONDITION = XMLString::transcode(relation_mention->getAttribute(XMLString::transcode("LEXICALCONDITION")));
									extent = relation_mention->getFirstElementChild();
									if(extent){
										charseq = extent->getFirstElementChild();
										if(charseq){
											sstream.clear();
											sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("START"))));
											sstream >> m_ACE_relation_mention.extent.START;
											sstream.clear();
											sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("END"))));
											sstream >> m_ACE_relation_mention.extent.END;
										}
										m_ACE_relation_mention.extent.charseq = XMLString::transcode(charseq->getTextContent());
									}
									relation_mention_argument = extent->getNextElementSibling();
									while(relation_mention_argument){
										if(!XMLString::compareIString(relation_mention_argument->getTagName(), XMLString::transcode("relation_mention_argument"))){
											ACE_relation_mention_arg m_ACE_relation_mention_arg;//***********
											m_ACE_relation_mention_arg.REFID = XMLString::transcode(relation_mention_argument->getAttribute(XMLString::transcode("REFID")));
											m_ACE_relation_mention_arg.ROLE = XMLString::transcode(relation_mention_argument->getAttribute(XMLString::transcode("ROLE")));
											extent = relation_mention_argument->getFirstElementChild();
											if(extent){
												charseq = extent->getFirstElementChild();
												if(charseq){
													sstream.clear();
													sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("START"))));
													sstream >> m_ACE_relation_mention_arg.extent.START;
													sstream.clear();
													sstream.str(XMLString::transcode(charseq->getAttribute(XMLString::transcode("END"))));
													sstream >> m_ACE_relation_mention_arg.extent.END;
												}
												m_ACE_relation_mention_arg.extent.charseq = XMLString::transcode(charseq->getTextContent());
											}
											m_ACE_relation_mention.arg_mention_mmap.insert(make_pair(m_ACE_relation_mention_arg.ROLE, m_ACE_relation_mention_arg));
										}//if... relation_mention_argument
										relation_mention_argument = relation_mention_argument->getNextElementSibling();
									}//while(relation_mention_argument)
									m_ACE_relation.relation_mention_v.push_back(m_ACE_relation_mention);
								}//else if...relation_mention
							relation_child_Element = relation_child_Element->getNextElementSibling();
						}//while(relation_mention}
						ACE_Relation_Info_v.push_back(m_ACE_relation);
					}//if(relation)
					ACE_Element = ACE_Element->getNextElementSibling();
				}//while(Relation)
			}//if...(document)
		}//if...source_file	
	}//if(Xroot)
	delete parser;
	delete errHandler;
	return true;
}


string SXMLer::Xercesc_Initialize()
{
	if(!Xercesc_Initialize_Flag){
		string rtnstr = "";
		ostringstream ostrsteam;
		ostrsteam.str("");
		try {
            XMLPlatformUtils::Initialize();
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            ostrsteam << "Error during initialization! :\n"
                 << message << "\n";
            XMLString::release(&message);
            rtnstr = ostrsteam.str();
			return "";
        }
		Xercesc_Initialize_Flag = true;
	}
	return "";
}


