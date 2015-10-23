/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a GPLv3+/MPLv2+ license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <string>
#include <sstream>
#include "ZenLib/Ztring.h"
#include "JS_Tree.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaConch {

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
String JsTree::format_from_XML(String& xml)
{
    String json(__T("["));
    std::string xml_not_unicode=Ztring(xml).To_UTF8();

    xmlDocPtr doc = xmlParseMemory(xml_not_unicode.c_str(), xml_not_unicode.length());
    if (!doc)
    {
        error = __T("The report given cannot be parsed");
        return String();
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root)
    {
        error = __T("No root node, leaving");
        return String();
    }

    xmlNodePtr child = root->children;
    bool new_sep = false;
    while (child)
    {
        find_block_node(child, new_sep, json);
        child = child->next;
    }

    json += String(__T("]"));

    xmlFreeDoc(doc);
    return json;
}

//---------------------------------------------------------------------------
void JsTree::find_block_node(xmlNodePtr node, bool& sep, String& json)
{
    std::string def("block");
    if (!node || node->type != XML_ELEMENT_NODE ||
        !node->name || def.compare((const char*)node->name))
        return;

    if (sep)
        json += __T(", ");
    else
        sep = true;

    json += __T("{\"type\":\"block\"");

    interpret_data_in_block(node, json);
    xmlNodePtr child = node->children;
    if (has_block_data(child))
    {
        json += __T(", \"children\":[");
        bool new_sep = false;
        while (child)
        {
            find_block_node(child, new_sep, json);
            find_data_node(child, new_sep, json);
            child = child->next;
        }
        json += __T("]");
    }

    json += __T("}");
}

//---------------------------------------------------------------------------
void JsTree::find_data_node(xmlNodePtr node, bool& sep, String& json)
{
    std::string def("data");
    if (!node || node->type != XML_ELEMENT_NODE ||
        !node->name || def.compare((const char*)node->name))
        return;

    if (sep)
        json += __T(", ");
    else
        sep = true;

    json += __T("{\"type\":\"data\"");

    interpret_data_in_data(node, json);

    xmlNodePtr child = node->children;
    if (has_block_data(child))
    {
        json += __T(", \"children\":[");
        bool new_sep = false;
        while (child)
        {
            find_block_node(child, new_sep, json);
            find_data_node(child, new_sep, json);
            child = child->next;
        }
        json += __T("]");
    }

    json += __T("}");
}

//---------------------------------------------------------------------------
bool JsTree::has_block_data(xmlNodePtr child)
{
    std::string block("block");
    std::string data("data");

    while (child)
    {
        if (child && child->type == XML_ELEMENT_NODE && child->name &&
            (block.compare((const char*)child->name) || data.compare((const char*)child->name)))
            return true;
        child = child->next;
    }

    return false;
}

//---------------------------------------------------------------------------
void JsTree::interpret_data_in_block(xmlNodePtr block, String& json)
{
    //Format: "text": "name[ - info] (size bytes)", "data": {"offset": "offset_hexa"}
    std::string offset;
    std::string name;
    std::string info;
    std::string size;

    xmlChar *offset_c = xmlGetNoNsProp(block, (const unsigned char*)"offset");
    if (offset_c != NULL)
        offset = (const  char *)offset_c;
    xmlChar *name_c = xmlGetNoNsProp(block, (const unsigned char*)"name");
    if (name_c != NULL)
        name = std::string((const  char *)name_c);
    xmlChar *info_c = xmlGetNoNsProp(block, (const unsigned char*)"info");
    if (info_c != NULL)
        info = std::string((const  char *)info_c);
    xmlChar *size_c = xmlGetNoNsProp(block, (const unsigned char*)"size");
    if (size_c != NULL)
        size = std::string((const  char *)size_c);

    //Block data
    json += __T(", \"text\":\"");
    json += Ztring().From_UTF8(name);
    if (info.length())
    {
        json += __T(" - ");
        json += Ztring().From_UTF8(info);
    }
    json += __T(" (");
    json += Ztring().From_UTF8(size);
    json += __T(" bytes)");
    json += __T("\"");

    //Block data
    json += __T(", \"data\":{");
    bool coma = false;
    if (offset_c)
    {
        interpret_offset(offset, coma, json);
        coma = true;
    }
    json += __T("}");
}

//---------------------------------------------------------------------------
void JsTree::interpret_data_in_data(xmlNodePtr data, String& json)
{
    //Format: "text": "name", "data": {"offset": "offset_hexa", "dataValue": "value (value_in_hexa)"}
    std::string offset; //Decimal to hexa
    std::string name;
    std::string value; //decimal + hexa if numerical

    xmlChar *offset_c = xmlGetNoNsProp(data, (const unsigned char*)"offset");
    if (offset_c != NULL)
        offset = (const char *)offset_c;
    xmlChar *name_c = xmlGetNoNsProp(data, (const unsigned char*)"name");
    if (name_c != NULL)
        name = std::string((const char *)name_c);

    xmlChar *value_c = xmlNodeGetContent(data);
    if (value_c != NULL)
        value = std::string((const char *)value_c);

    json += __T(", \"text\":\"");
    json += Ztring().From_UTF8(name);

    //Block data
    json += __T("\", \"data\":{");
    bool coma = false;
    if (offset_c)
    {
        interpret_offset(offset, coma, json);
        coma = true;
    }
    if (value.length())
    {
        interpret_value(value, coma, json);
        coma = true;
    }
    json += __T("}");
}

//---------------------------------------------------------------------------
void JsTree::interpret_offset(std::string& offset, bool coma, String& json)
{
    if (coma)
        json += __T(", ");
    json += __T("\"offset\":");

    // Not numerical
    std::size_t found = offset.find_first_not_of("0123456789.");
    if (found != std::string::npos)
        offset = "0";
    json += __T("\"0x");

    std::string hexa = decimal_to_hexa(offset);
    int zero = 8 - hexa.length();
    while (zero > 0)
    {
        json += __T("0");
        --zero;
    }
    json += Ztring().From_UTF8(hexa);

    json += __T("\"");
}

//---------------------------------------------------------------------------
void JsTree::interpret_value(std::string& value, bool coma, String& json)
{
    if (coma)
        json += __T(", ");
    json += __T("\"dataValue\":\"");

    json += Ztring().From_UTF8(value);

    // Not numerical
    std::size_t found = value.find_first_not_of("0123456789.");
    if (found != std::string::npos)
        value = "0";
    json += __T(" (0x");
    std::string hexa = decimal_to_hexa(value);
    json += Ztring().From_UTF8(hexa);
    json += __T(")\"");
}

//---------------------------------------------------------------------------
std::string JsTree::decimal_to_hexa(std::string str)
{
    std::stringstream ss;

    long long int val = strtoll(str.c_str(), NULL, 10);

    ss << std::hex << val;
    return ss.str();
}

}
