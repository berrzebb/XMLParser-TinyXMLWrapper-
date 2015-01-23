#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>

/// 전방선언
class NodeBase;
class NodeText;
class NodeDeclearation;
class NodeComment;
class NodeAttribute;
class NodeElement;
class NodeDocument;

typedef std::vector<NodeAttribute> AttributeList;
typedef std::vector<NodeBase*> NodeList;
/// Use Vistor Pattern
class NodeVisitor{
public:
	void visit(NodeBase& node);
	void visit(NodeText& node);
	void visit(NodeDeclearation& node);
	void visit(NodeComment& node);
	void visit(NodeAttribute& node);
	void visit(NodeElement& node);
	void visit(NodeDocument& node);
};
class NodeBase{
public:
	typedef NodeList::iterator iterator;
	typedef NodeList::const_iterator const_iterator;
	typedef NodeList::reverse_iterator reverse_iterator;
	typedef NodeList::const_reverse_iterator const_reverse_iterator;

	enum NodeType{
		null_type,
		declearation_type,
		comment_type,
		text_type,
		document_type,
		element_type,
		attribute_type,
	};
	NodeBase();
	NodeBase(const NodeType& type);
	NodeBase(const NodeBase& other);

	virtual NodeBase* Clone() const = 0;
	virtual std::string const GetName() const = 0;

	NodeType GetNodeType();

	const NodeBase& operator()();

	void accept(class NodeVisitor &v);

	NodeBase& ToBase();
	NodeText& ToText();
	NodeDeclearation& ToDeclaration();
	NodeComment& ToComment();
	NodeAttribute& ToAttribute();
	NodeElement& ToElement();
	NodeDocument& ToDocument();

private:
	NodeType Type;
};

class NodeText : public NodeBase{
public:
	NodeText();
	NodeText(const std::string Value);
	NodeText(const NodeText& other);
	~NodeText();

	std::string const GetValue() const;
	NodeText& Append(std::string Value);
	void operator()(std::string Value);
	void SetValue(const std::string Value);

	virtual NodeBase* Clone() const;
	virtual std::string const GetName() const;

private:
	std::string sValue;
};

class NodeDeclearation : public NodeBase{
public:
	NodeDeclearation();
	NodeDeclearation(const std::string version,const std::string encoding,const std::string standalone); 
	NodeDeclearation(const NodeDeclearation& other);
	~NodeDeclearation();

	std::string const GetVersion() const;
	std::string const GetEncoding() const;
	std::string const GetStandalone() const;

	virtual NodeBase* Clone() const;
	virtual std::string const GetName() const;

private:
	std::string sVersion;
	std::string sEncoding;
	std::string sStandalone;
};

class NodeComment : public NodeBase {
public:
	NodeComment();
	NodeComment(std::string Comment);
	NodeComment(const NodeComment& other);
	~NodeComment();

	NodeComment& append(std::string Comment);
	std::string const GetComment() const;

	void Clear();

	NodeComment& operator+=(std::string Comment);
	virtual NodeBase* Clone() const;
	virtual std::string const GetName() const;
private:
	std::string sComment;
};

class NodeAttribute : public NodeBase {
public:
	NodeAttribute();
	NodeAttribute(std::string key,std::string value);
	NodeAttribute(const NodeAttribute& other);
	~NodeAttribute();

	virtual std::string const GetName() const;
	std::string const GetValue() const;

	void SetValue(const std::string Value);
	NodeAttribute& operator=(std::string Value);
	bool operator==(const NodeAttribute& other);
	bool operator==(std::string Value);

	virtual NodeBase* Clone() const;
private:
	typedef std::pair<std::string,std::string> AttributeValue;
	AttributeValue Value;
};

class NodeElement : public NodeBase {
public:
	typedef AttributeList::iterator attr_iterator;
	NodeElement();
	NodeElement(const std::string Name);
	NodeElement(const NodeElement& other);
	~NodeElement();
	NodeElement& AddNode(NodeBase* Child);
	NodeElement& AddNode(const std::string Name);
	NodeBase::iterator FindNode(const std::string Name);
	NodeBase* FindNode(const NodeBase::NodeType Type);
	NodeBase::iterator FirstNode();
	NodeBase::iterator NextNode();
	NodeBase::iterator LastNode();

	void Clear();

	NodeElement& AddAttribute(NodeAttribute& attribute);
	NodeElement& AddAttribute(const std::string Name,const std::string Value);
	attr_iterator FindAttribute(const std::string Name);
	attr_iterator FirstAttribute();
	attr_iterator NextAttribute();
	attr_iterator LastAttribute();

	NodeElement& AddText(const std::string Value);

	virtual std::string const GetName() const;

	virtual NodeBase* Clone() const;

	NodeElement& operator()(std::string Name);
	NodeAttribute& operator[](std::string Name);
	NodeElement& operator=(std::string Name);
private:
	std::string _Name;
	AttributeList _Attribute;
	NodeList _Child;
	NodeBase::iterator iter;
	attr_iterator attr_iter;
};

class NodeDocument : public NodeBase {
public:
	typedef NodeList::iterator elem_iterator;

	NodeDocument();
	NodeDocument(const std::string Doc);
	NodeDocument(const NodeDocument& Doc);
	~NodeDocument();

	bool LoadFile(const std::string Doc);
	NodeElement& GetRootNode();

	bool SaveFile(const std::string Doc);
	NodeDocument& AddNode(NodeBase* Node);
	NodeDocument& AddNode(NodeBase* parent,const NodeBase* Child);

	virtual NodeBase* Clone() const;
	virtual std::string const GetName() const;

	NodeElement& operator()();
private:
	NodeElement Root;
};

