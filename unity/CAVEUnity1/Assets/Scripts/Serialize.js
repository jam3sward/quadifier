#pragma strict

import System;
import System.Text;
import System.IO;
import System.Xml;
import System.Xml.Serialization;

//---------------------------------------------------------

function UTF8ByteArrayToString(byteArray : byte[])
{     
	var encode : UTF8Encoding  = new UTF8Encoding();
	var result : String = encode.GetString(byteArray);
	return result;
}

//---------------------------------------------------------

function stringToUTF8ByteArray(text : String)
{
	var encode : UTF8Encoding  = new UTF8Encoding();
	var result : byte[] = encode.GetBytes(text);
	return result;
}

//---------------------------------------------------------

function ignoreAttributes()
{
	// create an XmlIgnore attribute
	var ignore = new XmlAttributes();
	ignore.XmlIgnore = true;

	// create attribute overrides
	var attrib = new XmlAttributeOverrides();
	
	// ignore various attributes of existing Unity classes
	// when serializing to XML
	
	// Quaternion
	attrib.Add(typeof(Quaternion), "eulerAngles", ignore);
	// Rect
	attrib.Add(typeof(Rect), "center", ignore);
	attrib.Add(typeof(Rect), "xMin", ignore);
	attrib.Add(typeof(Rect), "yMin", ignore);
	attrib.Add(typeof(Rect), "xMax", ignore);
	attrib.Add(typeof(Rect), "yMax", ignore);
	
	// return the attribute overrides, which can then be passed to
	// the serializer to prevent the attributes being serialized
	return attrib;
}

//---------------------------------------------------------

function serializeObject(object : Object)
{
	var stream : MemoryStream = new MemoryStream();
	var writer : System.Xml.XmlTextWriter =
		new System.Xml.XmlTextWriter(stream, Encoding.UTF8);
	
	// attributes to ignore when serializing to XML
	var ignore = ignoreAttributes();
	
	var serial : XmlSerializer = new XmlSerializer(typeof(object), ignore);
	serial.Serialize(writer, object);
	stream = writer.BaseStream as MemoryStream;
	
	var result : String  = null;
	result = UTF8ByteArrayToString(stream.ToArray());
	return result;
}

//---------------------------------------------------------

function deserializeObject(text : String, object :Object)   
{
	var buffer : byte[] = stringToUTF8ByteArray(text);
	
	var stream : MemoryStream  = new MemoryStream(buffer);
	
	var writer : System.Xml.XmlTextWriter =
		new System.Xml.XmlTextWriter(stream, Encoding.UTF8);
	
	var serial : XmlSerializer = new XmlSerializer(typeof(object));	
	return serial.Deserialize(stream);
}

//---------------------------------------------------------

function saveXML(object : Object, fileName : String)
{
	var fileInfo : FileInfo = new FileInfo(fileName);
	if (fileInfo.Exists) fileInfo.Delete();	
	
	var file : StreamWriter;
	file = fileInfo.CreateText();
	file.Write( serializeObject(object) );
	file.Close();
}

//---------------------------------------------------------

function loadXML(object :Object, fileName : String)
{
   var file : StreamReader = File.OpenText(fileName);
   var text : String = file.ReadToEnd();
   file.Close();
   return deserializeObject(text, object);
}

//---------------------------------------------------------
