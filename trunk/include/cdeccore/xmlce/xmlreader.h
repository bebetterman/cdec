// -------------------------------------------------------------------------- //
//	文件名		：	xmlreader.h
//	创建者		：	杨钢
//	创建时间	：	4/10/2008 11:38:53 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

// Xml 解析器的简单接口
// 当回调接口遇到错误时，可以通过异常返回，这会导致解析过程终止，并且异常被传出
interface ISaxAccepter: Object
{
	DECLARE_REF_CLASS(ISaxAccepter)

	virtual void BeginDocument() = 0;
	virtual void EndDocument() = 0;
	virtual void BeginElement(stringx name) = 0;
	virtual void EndElement(stringx name) = 0;
	virtual void AddAttribute(stringx name, stringx value) = 0;
	virtual void AddValue(stringx value) = 0;
};

// -------------------------------------------------------------------------- //

class XmlReader
{
protected:
	static void VerifyEncoding(ref<StreamReader> pReader);

public:
	static void ParseText(ref<ISaxAccepter> accpt, stringx xmlText);
	static void Parse(ref<ISaxAccepter> accpt, ref<TextReader> tr);
	static void Parse(ref<ISaxAccepter> accpt, stringx fileName);
	static void Parse(ref<ISaxAccepter> accpt, ref<Stream> stream);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
