#include "StdAfx.h"
#include "XMLParser.h"
#include "tinyxml2\tinyxml.h"
#include <algorithm>
/*********
*Node Base
**********/
NodeBase::NodeBase() : Type(null_type)
{

}

NodeBase::NodeBase(const NodeBase& other) : Type(other.Type)
{

}

NodeBase::NodeBase(const NodeType& type) : Type(type)
{

}

NodeBase::NodeType NodeBase::GetNodeType()
{
	return Type;
}

const NodeBase& NodeBase::operator()()
{
	return *this;
}

void NodeBase::accept(class NodeVisitor &v)
{
	v.visit(*this);
}

NodeBase& NodeBase::ToBase()
{
	return static_cast<NodeBase&>(*this);
}
NodeText& NodeBase::ToText()
{
	return static_cast<NodeText&>(*this);
}

NodeDeclearation& NodeBase::ToDeclaration()
{
	return static_cast<NodeDeclearation&>(*this);
}

NodeComment& NodeBase::ToComment()
{
	return static_cast<NodeComment&>(*this);
}

NodeAttribute& NodeBase::ToAttribute()
{
	return static_cast<NodeAttribute&>(*this);
}

NodeElement& NodeBase::ToElement()
{
	return static_cast<NodeElement&>(*this);
}

NodeDocument& NodeBase::ToDocument()
{
	return static_cast<NodeDocument&>(*this);
}

/*********
*Node Text
**********/
NodeText::NodeText() : NodeBase(text_type)
{

}
NodeText::NodeText(const std::string Value) : NodeBase(text_type),sValue(Value)
{

}

NodeText::NodeText(const NodeText& other) : NodeBase(text_type),sValue(other.sValue)
{

}
NodeText::~NodeText()
{

}

std::string const NodeText::GetValue() const
{
	return sValue;
}

void NodeText::operator()(std::string Value)
{
	sValue = Value;
}

NodeText& NodeText::Append(std::string Value)
{
	sValue+=Value;
	return *this;
}

NodeBase* NodeText::Clone() const
{
	return new NodeText(*this);
}

std::string const NodeText::GetName() const
{
	return "";
}

void NodeText::SetValue(const std::string Value)
{
	sValue = Value;
}

/******************
*Node Declearation
*******************/
NodeDeclearation::NodeDeclearation() : NodeBase(declearation_type)
{

}

NodeDeclearation::NodeDeclearation(const std::string version,const std::string encoding,const std::string standalone) : NodeBase(declearation_type), sVersion(version),sEncoding(encoding),sStandalone(standalone)
{

}

NodeDeclearation::NodeDeclearation(const NodeDeclearation& other) : NodeBase(declearation_type), sVersion(other.sVersion), sEncoding(other.sEncoding), sStandalone(other.sStandalone)
{

}

NodeDeclearation::~NodeDeclearation()
{

}

std::string const NodeDeclearation::GetVersion() const
{
	return sVersion;
}

std::string const NodeDeclearation::GetEncoding() const
{
	return sEncoding;
}

std::string const NodeDeclearation::GetStandalone() const
{
	return sStandalone;
}

NodeBase* NodeDeclearation::Clone() const
{
	return new NodeDeclearation(*this);
}

std::string const NodeDeclearation::GetName() const
{
	return "";
}

/******************
*Node Comment
*******************/
NodeComment::NodeComment() : NodeBase(comment_type)
{

}
NodeComment::NodeComment(std::string Comment) :NodeBase(comment_type),sComment(Comment)
{

}

NodeComment::NodeComment(const NodeComment& other) : NodeBase(comment_type),sComment(other.sComment)
{

}

NodeComment::~NodeComment()
{

}

NodeComment& NodeComment::append(std::string Comment)
{
	sComment+=Comment;
	return *this;
}

std::string const NodeComment::GetComment() const
{
	return sComment;
}

NodeBase* NodeComment::Clone() const
{
	return new NodeComment(*this);
}

std::string const NodeComment::GetName() const
{
	return "";
}

void NodeComment::Clear()
{
	sComment.clear();
}

NodeComment& NodeComment::operator+=(std::string Comment)
{
	return append(Comment);
}

/******************
*Node Attribute
*******************/
NodeAttribute::NodeAttribute() :NodeBase(attribute_type)
{

}

NodeAttribute::NodeAttribute(std::string key,std::string value) : NodeBase(attribute_type), Value(std::make_pair<std::string,std::string>(key,value))
{

}

NodeAttribute::NodeAttribute(const NodeAttribute& other) : NodeBase(attribute_type),Value(other.Value)
{

}

NodeAttribute::~NodeAttribute()
{

}

std::string const NodeAttribute::GetName() const
{
	return Value.first;
}

std::string const NodeAttribute::GetValue() const
{
	return Value.second;
}

NodeBase* NodeAttribute::Clone() const
{
	return new NodeAttribute(*this);
}

void NodeAttribute::SetValue(const std::string _Value)
{
	Value.second = _Value;
}

NodeAttribute& NodeAttribute::operator=(const std::string _Value)
{
	SetValue(_Value);
	return *this;
}

bool NodeAttribute::operator==(std::string _Value)
{
	return Value.second == _Value;
}

bool NodeAttribute::operator==(const NodeAttribute& other)
{
	return Value.second == other.Value.second;
}

/******************
*Node Element
*******************/
NodeElement::NodeElement() : NodeBase(element_type)
{

}

NodeElement::NodeElement(const std::string Name) : NodeBase(element_type),_Name(Name)
{

}

NodeElement::NodeElement(const NodeElement& other) : NodeBase(element_type),_Name(other._Name),_Attribute(other._Attribute),_Child(other._Child)
{

}

NodeElement::~NodeElement()
{
}

NodeElement& NodeElement::AddNode(NodeBase* Child)
{
	_Child.emplace_back(Child);
	return *this;
}

NodeElement& NodeElement::AddNode(const std::string Name)
{
	NodeElement* Node = new NodeElement(Name);
	_Child.emplace_back(Node);
	return *Node;
}

NodeBase::iterator NodeElement::FindNode(const std::string Name)
{
	NodeBase::iterator i = std::find_if(_Child.begin(),_Child.end(),[Name](NodeBase* Node){
		return Node->GetName() == Name;
	});
	return i;
}
NodeBase* NodeElement::FindNode(const NodeBase::NodeType Type){
	NodeBase::iterator i = std::find_if(_Child.begin(),_Child.end(),[Type](NodeBase* Node){
		return Node->GetNodeType() == Type;
	});
	return (*iter);
}
NodeBase::iterator NodeElement::FirstNode()
{
	iter = _Child.begin();
	return iter;
}
NodeBase::iterator NodeElement::NextNode()
{
	return iter++;
}

NodeBase::iterator NodeElement::LastNode()
{
	iter = _Child.end();
	return iter;
}

NodeElement& NodeElement::AddAttribute(NodeAttribute& attribute)
{
	_Attribute.emplace_back(attribute);
	return *this;
}

NodeElement& NodeElement::AddAttribute(const std::string Name,const std::string Value)
{
	_Attribute.emplace_back(NodeAttribute(Name,Value));
	return *this;
}
NodeElement::attr_iterator NodeElement::FindAttribute(const std::string Name)
{
	attr_iterator i = std::find_if(_Attribute.begin(),_Attribute.end(),[Name](NodeAttribute const& Attribute){
		return Attribute.GetName() == Name;
	});
	return i;
}

NodeElement::attr_iterator NodeElement::FirstAttribute()
{
	attr_iter = _Attribute.begin();
	return attr_iter;
}

NodeElement::attr_iterator NodeElement::NextAttribute()
{
	return attr_iter++;
}

NodeElement::attr_iterator NodeElement::LastAttribute()
{
	attr_iter = _Attribute.end();
	return attr_iter;
}

std::string const NodeElement::GetName() const
{
	return _Name;
}

NodeBase* NodeElement::Clone() const
{
	return new NodeElement(*this);
}

NodeElement& NodeElement::operator()(std::string Name)
{
	NodeBase::iterator iter= FindNode(Name);
	return (*iter)->ToElement();
}
NodeAttribute& NodeElement::operator[](std::string Name)
{
	attr_iterator iter = FindAttribute(Name);
	if(iter == LastAttribute()){
		_Attribute.emplace_back(NodeAttribute(Name,""));
		iter = FindAttribute(Name);
	}
	return (*iter).ToAttribute();
}
NodeElement& NodeElement::operator=(std::string Value)
{
	return AddText(Value);
}

NodeElement& NodeElement::AddText(const std::string Value)
{
	NodeBase::iterator i = std::find_if(_Child.begin(),_Child.end(),[](NodeBase* Node){
		return Node->GetNodeType() == NodeBase::text_type;
	});	
	if(i != LastNode()){
		(*i)->ToText().SetValue(Value);
	}else{
		_Child.emplace_back(new NodeText(Value));
	}
	return *this;
}

void RemoveElement(NodeElement* Elem){
	for(NodeBase::iterator iter = Elem->FirstNode(); iter != Elem->LastNode(); iter++)
	{
		NodeBase* Base = (*iter);
		if(Base->GetNodeType() == NodeBase::element_type){
			RemoveElement(&Base->ToElement());
		}
		delete Base;
		Base = NULL;
	}
}

void NodeElement::Clear()
{
	RemoveElement(this);
}


/******************
*Node Element
*******************/
NodeDocument::NodeDocument() : NodeBase(document_type)
{

}

NodeDocument::NodeDocument(const std::string Doc) : NodeBase(document_type)
{
	LoadFile(Doc);
}

NodeDocument::NodeDocument(const NodeDocument& Doc) : NodeBase(document_type),Root(Doc.Root)
{

}
NodeDocument::~NodeDocument()
{
	RemoveElement(&Root);
}
void searchXMLData(TiXmlElement* pElem,NodeElement& Node,NodeElement& Child){
	TiXmlHandle hRoot(0);
	TiXmlElement* pSub = pElem;
	TiXmlAttribute* pAttr = NULL;
	NodeElement curNode;
	hRoot = TiXmlHandle(pSub);
	pSub = hRoot.FirstChildElement().Element();

	if(!pSub) return;

	while(pSub)
	{
		/// 노드 생성
		Child = NodeElement(pSub->Value());
		/// 속성 추가
		pAttr = pSub->FirstAttribute();
		while(pAttr){
			Child[pAttr->Name()] = pAttr->Value();
			pAttr = pAttr->Next();
		}

		/// 데이터 추가
		char* pszText = (char*)pSub->GetText();
		if(pszText){
			Child = pszText;
		}
		searchXMLData(pSub,Child,curNode);
		Node.AddNode(Child.Clone());

		pSub = pSub->NextSiblingElement();
	}
}
bool NodeDocument::LoadFile(const std::string Doc)
{
	TiXmlDocument doc;
	bool result = doc.LoadFile(Doc.c_str());

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pRoot = hDoc.FirstChildElement().Element();
	if(!pRoot) return false;

	NodeDeclearation decl("1.0","UTF-8","");
	AddNode(decl.Clone());

	NodeComment Comment;
	Comment+=" \nDescription\n";
	Comment+="An RTI Data Distribution Service QoS Profile that provides high throughput\n";
	Comment+="for streaming reliable data.\n\n";
	Comment+="This profile depends primarily on:\n\n";
	Comment+="Data writer:\n";
	Comment+="  - batch:    combining multiple samples into a single network packet to\n";
	Comment+="              increase throughput\n";
	Comment+="  - protocol: send heartbeats to readers more frequently to cache levels low\n\n";
	Comment+="Data reader:\n";
	Comment+="  - protocol: respond more aggressively to heartbeats with positive or\n";
	Comment+="              negative acknowledgements to speed up repairs of lost packets\n\n";
	Comment+="Domain participant:\n";
	Comment+="  - Increased transport buffer sizes to efficiently send and receive many\n";
	Comment+="    large packets\n\n";

	AddNode(Comment.Clone());
	AddNode(new NodeComment(" ================================================================= "));
	AddNode(new NodeComment(" Throughput QoS Profile                                            "));
	AddNode(new NodeComment(" ================================================================= "));

	Comment.Clear();
	Comment+="\nYour XML editor may be able to provide validation and auto-completion services\n";
	Comment+="as you type. To enable these services, replace the opening tag of this\n";
	Comment+="document with the following, and update the absolute path as appropriate for\n";
	Comment+="your installation:\n  \n";

	Comment+="<dds xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	Comment+="     xsi:noNamespaceSchemaLocation=\"C:/RTI/ndds.4.5e/resource/qos_profiles_4.5e/schema/rti_dds_qos_profiles.xsd\">\n";
	AddNode(Comment.Clone());
	/// 노드 생성
	NodeElement RootElem = NodeElement(pRoot->Value());
	/// 속성 추가
	TiXmlAttribute* pAttr = pRoot->FirstAttribute();
	while(pAttr){
		RootElem[pAttr->Name()] = pAttr->Value();
		pAttr = pAttr->Next();
	}

	/// 데이터 추가
	char* pszText = (char*)pRoot->GetText();
	if(pszText)
		RootElem.AddText(pszText);
	NodeElement Elem;
	searchXMLData(pRoot,RootElem,Elem);
	AddNode(RootElem.Clone());
	return result;
}

NodeElement& NodeDocument::GetRootNode()
{
	return Root;
}

TiXmlElement* GetElement(TiXmlElement* current,NodeElement* Elem)
{
	if(Elem == NULL) return current;
	for(NodeBase::iterator iter = Elem->FirstNode(); iter != Elem->LastNode(); iter++)
	{
		NodeBase* Base = (*iter);
		if(Base->GetNodeType() == NodeBase::element_type){
			TiXmlElement* elem = new TiXmlElement(Base->ToElement().GetName());
			for(NodeElement::attr_iterator attr_iter = Base->ToElement().FirstAttribute(); attr_iter != Base->ToElement().LastAttribute(); attr_iter++)
			{
				elem->SetAttribute((*attr_iter).GetName(),(*attr_iter).GetValue());
			}
			current->LinkEndChild(GetElement(elem,&Base->ToElement()));
		}else if(Base->GetNodeType() == NodeBase::text_type){
			TiXmlText* text = new TiXmlText(Base->ToText().GetValue());
			current->LinkEndChild(text);
		}
	}
	return current;
}
bool NodeDocument::SaveFile(std::string Doc)
{
	TiXmlDocument doc;
	NodeBase* Base;
	TiXmlElement* elem,*cur;
	TiXmlAttribute* attr;

	for(NodeBase::iterator iter = Root.FirstNode(); iter != Root.LastNode(); ++iter)
	{
		Base = (*iter);
		switch(Base->GetNodeType()){
		case null_type:
			break;
		case declearation_type:
			doc.LinkEndChild(new TiXmlDeclaration(Base->ToDeclaration().GetVersion(),Base->ToDeclaration().GetEncoding(),Base->ToDeclaration().GetStandalone()));
			break;
		case comment_type:
			doc.LinkEndChild(new TiXmlComment(Base->ToComment().GetComment().c_str()));
			break;
		case element_type:
			elem = new TiXmlElement(Base->ToElement().GetName());
			for(NodeElement::attr_iterator attr_iter = Base->ToElement().FirstAttribute(); attr_iter != Base->ToElement().LastAttribute(); attr_iter++)
			{
				elem->SetAttribute((*attr_iter).GetName(),(*attr_iter).GetValue());
			}

			doc.LinkEndChild(GetElement(elem,&Base->ToElement()));
			break;
		}
	}
	doc.SaveFile(Doc);
	return true;
}

NodeDocument& NodeDocument::AddNode(NodeBase* Node)
{
	Root.AddNode(Node);
	return *this;
}

NodeDocument& NodeDocument::AddNode(NodeBase* parent,const NodeBase* Child)
{
	return *this;
}

NodeElement& NodeDocument::operator()()
{
	return Root;
}

NodeBase* NodeDocument::Clone() const
{
	return new NodeDocument(*this);
}

std::string const NodeDocument::GetName() const
{
	return Root.GetName();
}

void NodeVisitor::visit(NodeBase& node)
{
		switch(node.GetNodeType())
		{
		case NodeBase::text_type:
			visit(node.ToText());
			break;
		case NodeBase::null_type:
			break;
		case NodeBase::declearation_type:
			visit(node.ToDeclaration());
			break;
		case NodeBase::comment_type:
			visit(node.ToComment());
			break;
		case NodeBase::attribute_type:
			visit(node.ToAttribute());
			break;
		case NodeBase::element_type:
			visit(node.ToElement());
			break;
		case NodeBase::document_type:
			visit(node.ToDocument());
			break;
		}
}

void NodeVisitor::visit(NodeText& node)
{
	printf("Text : %s\n",node.GetValue().c_str());
}

void NodeVisitor::visit(NodeDeclearation& node)
{
	printf("Declaration XML Version %s Encoding %s \n",node.GetVersion().c_str(),node.GetEncoding().c_str());
}

void NodeVisitor::visit(NodeComment& node)
{
	printf("%s\n",node.GetComment().c_str());
}

void NodeVisitor::visit(NodeAttribute& node)
{
	printf("Attribute Name : %s Value : %s\n",node.GetName().c_str(),node.GetValue().c_str());
}

void NodeVisitor::visit(NodeElement& node)
{
	for(NodeBase::iterator iter = node.FirstNode(); iter != node.LastNode(); iter++)
	{
		if((*iter)->GetNodeType() == NodeBase::element_type){
			printf("Node Name : %s\n",node.GetName().c_str());
			for(NodeElement::attr_iterator attr_iter = (*iter)->ToElement().FirstAttribute(); attr_iter != (*iter)->ToElement().LastAttribute(); attr_iter++)
			{
				(*attr_iter).accept(*this);
			}
		}
		(*iter)->accept(*this);
	}
}

void NodeVisitor::visit(NodeDocument& node)
{
	node.GetRootNode().accept(*this);
}
