
#include "pdfwriter.h"

#include "po_stream.h"
#include "embedded_icc.h"

#include <cstdarg>
#include <cstdio>
#include <sstream> 


PdfWriter::PdfWriter(std::ostream* ostream)
   : currentObject(1) 
   , streampos(0)
   , catalog(0) // root
    , _xmp(0)
    ,_outputIntents(0)
{
    stream = new Postream(ostream);
}


int PdfWriter::addXrefEntry(int object, bool printostr)
{
    if (object < 0)
        object = requestObject();

    if (object >= xrefPositions.size())
        xrefPositions.resize(object + 1);

    xrefPositions[object] = streampos;
    if (printostr) {
        xprintf("%d 0 obj\n", object);
    }

    return object;
}



void PdfWriter::write( std::string &data ) {
    stream->writeRawData((unsigned char*)data.data(), data.size());
    streampos += data.size();
}

void PdfWriter::writePage() {

    uint32_t resources = requestObject();

    int obj = addXrefEntry(-1, true);
    pages.push_back(obj);
    xprintf("<< /Type /Page\n" //The name /Page is associated with the dictionary key /Type 
        "/MediaBox [0 0 612 792]\n"
        "/Resources %d 0 R\n"
        "/Parent %d 0 R\n"
        "/Contents [%d 0 R]\n"
        ">>\n"
        "endobj\n"
        , resources
        , pageRoot
        , _pagecontent
    );

    writeFonts(resources);
}

// hello with a build-in font
void PdfWriter::pageContentHello() {

    std::ostringstream os;

    os << "1. 0. 0. 1. 50. 700. cm\n"; // Position at (50, 700)
    os << "BT\n";
    os << "/F0 36. Tf\n";
    // os << "(abab) Tj\n";     // Place the text string
    os << "(Hello) Tj\n";     // Place the text string
                         //   os << "<FEFF0061006200610062> Tj\n"; 
                         //   os << "(\141\142\141\142) Tj\n";     // Place the text string
    os << "ET";

    std::string stream = os.str();


    _pagecontent = addXrefEntry(-1, true);
    xprintf(            // The page contents stream
        "<< /Length %d >>\n"        // stream len
        "stream"                 // Beginning of stream
        "\n%s\n"
        "endstream\n"              // End of stream
        "endobj\n",
        stream.size(), stream.c_str());
}

// content with t3 font
void PdfWriter::pageContent() {

    std::ostringstream os;

    os << "1. 0. 0. 1. 50. 700. cm\n"; // Position at (50, 700)
    os << "BT\n";
    os << "/F" << this->type3FontID << " 36. Tf\n";
 //   os << "(abab) Tj\n";     // Place the text string
   // os << "() Tj\n";     // Place the text string
//   os << "<FEFF0061006200610062> Tj\n"; 
//    os << "(\128\130) Tj\n";     // Place the text string
    os << "<808182> Tj\n";     // Place the text string
    os << "ET";

    std::string stream = os.str();


    _pagecontent = addXrefEntry(-1,true);
    xprintf(            // The page contents stream
        "<< /Length %d >>\n"        // stream len
        "stream"                 // Beginning of stream
        "\n%s\n"
        "endstream\n"              // End of stream
        "endobj\n",
        stream.size(), stream.c_str());
}

void PdfWriter::addType3Font() {

    int encoding = addXrefEntry(-1, true);
    xprintf("<< \n"
        "/Type /Encoding\n"
        "/Differences [ 128 /square /cross /triangle ]\n"
        ">>\n"
        "endobj\n");




    std::string item1("1000 0 0 0 750 750 d1\n"
        "0 0 750 750 re\n"
        "f");

    int proc1 = addXrefEntry(-1, true);
    xprintf("<<\n /Length %d >>\n"
        "stream"
        "\n%s\n"
        "endstream\n"
        "endobj\n",
        item1.size(),
        item1.c_str()
    );


    // cross
    std::string item2("1000 0 0 0 750 750 d1\n"
        "0 0 m\n"
        "750 750 l\n"
        "0 750 l\n"
        "f");

    int proc2 = addXrefEntry(-1, true);
    xprintf("<<\n"
        "/Length %d >>\n"
        "stream"
        "\n%s\n"
        "endstream\n"
        "endobj\n",
        item2.size(),
        item2.c_str()
    );

    // triangle
    std::string item3("1000 0 0 0 750 750 d1\n"
        "0 0 m\n"
        "375 750 l\n"
        "750 0 l\n"
        "f");

    int proc3 = addXrefEntry(-1, true);
    xprintf("<<\n"
        "/Length %d >>\n"
        "stream"
        "\n%s\n"
        "endstream\n"
        "endobj\n",
        item3.size(),
        item3.c_str()
    );

    int charProcs = addXrefEntry(-1, true);
    xprintf("<<\n"
        "/square %d 0 R\n"
        "/cross %d 0 R\n"
        "/triangle %d 0 R\n"
        ">>\n"
        "endobj\n", 
        proc1, 
        proc2,
        proc3);


    // CMap  
    // map unicode
    int cmapOID = addXrefEntry(-1, true);
    {



        std::ostringstream os;





     //  os << "/CIDInit /ProcSet findresource begin\n";
     //  os << "12 dict begin begincmap /CIDSystemInfo << /Registry (OPExtFont0) /Ordering (Identity) /Supplement 0 >>\n";
     //  os << "def\n";
     //  os << "1 begincodespacerange\n";
     //  os << "<0001> <FFFF>\n";
     //  os << "endcodespacerange\n";
     //  os << "/CMapName  /OPExtFont0 def /CMapType 2 def\n";
     //  os << "0 beginbfchar\n";
     //  os << "endbfchar\n";
     //  os << "1 beginbfrange\n";
     //  os << "<0001> <FFFF> <0001>\n";
     //  os << "endbfrange\n";
     //  os << "endcmap CMapName currentdict /CMap defineresource pop end\n";
     //  os << "end\n";

        // <number of elemens> beginbfchar

        os << "/CIDInit /ProcSet findresource begin\n";
        os << "12 dict begin begincmap /CIDSystemInfo << /Registry (OPExtFont0) /Ordering (Identity) /Supplement 0 >>\n";
        os << "def\n";
        os << "1 begincodespacerange\n";
        os << "<00> <FF>\n";
        os << "endcodespacerange\n";
        os << "/CMapName  /OPExtFont0 def /CMapType 2 def\n";
        os << "0 beginbfchar\n";
        os << "endbfchar\n";

        os << "3 beginbfchar\n";
        os << "<80> <20AC>\n";
        os << "<81> <0020>\n";
        os << "<82> <201A>\n";
        os << "endbfchar\n";



  //   os << "1 beginbfrange\n";
  //   os << "<80> <80> <20AC>\n";
  //   os << "endbfrange\n";
  //
  //   os << "2 beginbfrange\n";
  //   os << "<81> <81> <0020>\n";
  //   os << "endbfrange\n";
  //
  //   os << "3 beginbfrange\n";
  //   os << "<82> <82> <201A>\n";
  //   os << "endbfrange\n";

        os << "endcmap CMapName currentdict /CMap defineresource pop end\n";
        os << "end\n";

        std::string stream = os.str();

        xprintf("<<\n"
            "/Type /CMap\n"
            "/Length %d >>\n"
            "stream"
            "\n%s\n"
            "endstream\n"
            "endobj\n",
            stream.size(),
            stream.c_str()
        );

    }

    type3FontID = addXrefEntry(-1, true);

    xprintf("<< /Type /Font\n"
        "/Subtype /Type3\n"
        "/FontBBox [ 0 0 750 750 ]\n"
        "/FontMatrix [ 0.001 0 0 0.001 0 0 ]\n"
        "/CharProcs %d 0 R\n"
        "/Encoding %d 0 R\n"
        "/FirstChar 128\n"
        "/LastChar 130\n"
        "/Widths [ 1000 1000 ]\n"
        "/ToUnicode %d 0 R\n"
        ">>\n"
        "endobj\n"
        , charProcs
        , encoding
        , cmapOID
    );

    fonts.push_back(type3FontID);
}

void PdfWriter::writePageRoot() {

    addXrefEntry(pageRoot,true);

    xprintf(
        "<< /Type /Pages\n" // It's a page list
        "/ProcSet [/PDF /Text /ImageB /ImageC]\n"
        "/Count 1 \n"       // There is one page
        "/Kids[");

    int size = pages.size();
    for (int i = 0; i < size; ++i) {
        xprintf("%d ", pages[i]);
    }

    xprintf("0 R]\n"
        ">>\n"
        "endobj\n");


}

void PdfWriter::end() {



    writeTail();
}
void PdfWriter::writeFonts(int fontres) {


    addXrefEntry(fontres, true);


    if (fonts.size() > 0) {

        xprintf("<<\n /Font <<\n");
        for (int i = 0; i < fonts.size(); ++i)
            xprintf("/F%d %d 0 R\n", fonts[i], fonts[i]);
        xprintf(">>\n");

        xprintf(">>\n"
            "endobj\n");
    }

    else {

        xprintf(
            "<< /Font\n"// The font dictionary
            "<< /F0\n" // Just one font, called /F0
            "<< /Type /Font\n" // These three lines reference the built-in font Times Italic
            "/BaseFont /Times-Italic\n"
            "/Subtype /Type1  >>\n"
            ">>\n"
            ">>\n"
            "endobj\n");
    }

}

void PdfWriter::writeHeader() {

    addXrefEntry(0, false);
    std::string versionInfo("timpdftk 1.0");

    xprintf("%%PDF-1.4\n%%\xE2\xE3\xCF\xD3\n");
    xprintf("%%%%%s\n", versionInfo.data());

    int structTreeRootOID = addXrefEntry(-1, true);
    {        
        xprintf("<<\n /Type /StructTreeRoot \n");
        xprintf(">>\nendobj\n");
    }

    writeXMP();
    writeOutputIntents();

    catalog = addXrefEntry(-1,true);
    pageRoot = requestObject();

    xprintf("<<\n"
        "/Type /Catalog\n"
        "/Pages %d 0 R\n"
        "/StructTreeRoot %d 0 R\n"
        "/OutputIntents [%d 0 R]\n"
        "/Metadata %d 0 R\n"
        "/MarkInfo << /Marked true >>\n"
        ">>\n"
        "endobj\n"
        , pageRoot
        , structTreeRootOID
        , _outputIntents
        , _xmp
    );
}

/*
xref entry
nnnnnnnnnn ggggg n eol

nnnnnnnnnn is a 10-digit byte offset
ggggg is a 5-digit generation number
n is a literal keyword identifying this as an in-use entry
eol is a 2-character end-of-line sequence

Thus, the overall length of the entry is always exactly 20
bytes.

*/
void PdfWriter::writeTail()
{
    writePage();
  //  writeFonts();
    writePageRoot();
    addXrefEntry(xrefPositions.size(), false);
    xprintf("xref\n"
        "0 %d\n"
        "%010d 65535 f \n", xrefPositions.size() - 1, xrefPositions[0]);

    for (int i = 1; i < xrefPositions.size() - 1; ++i) {
        xprintf("%010d 00000 n \n", xrefPositions[i]);
    }

    std::string trailerid("75ff22189ceac848dfa2afec93deee03");

    xprintf("trailer\n"
        "<<\n"
        "/Size %d\n"
    //    "/Info %d 0 R\n"
        "/Root %d 0 R\n"
        "/ID [<%s><%s>]\n"
        ">>\n"
        "startxref\n%d\n"
        "%%%%EOF\n",
        xrefPositions.size() - 1, 
   //     info, 
        catalog,
        trailerid.data(), trailerid.data(),
        xrefPositions.back());
}


// For strings up to 10000 bytes only !
void PdfWriter::xprintf(const char* fmt, ...)
{
    if (!stream)
        return;

    const int msize = 10000;
    unsigned char buf[msize];

    va_list args;
    va_start(args, fmt);
    int bufsize = vsnprintf( (char*)buf , msize, fmt, args);

    //Q_ASSERT(bufsize<msize);

    va_end(args);

    stream->writeRawData(buf, bufsize);
    streampos += bufsize;
}

void PdfWriter::writeRawData(unsigned char* data, int dataSize) {

    stream->writeRawData(data, dataSize);
    streampos += dataSize;
}

void PdfWriter::writeXMP() {

    std::ostringstream os;

    os << "<?xpacket begin='\xC3\xAF\xC2\xBB\xC2\xBF' id='W5M0MpCehiHzreSzNTczkc9d'?>\n";
    os << "<x:xmpmeta xmlns:x=\"adobe:ns:meta/\">\n";
    os << "<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n";
    os << "<rdf:Description xmlns:dc=\"http://purl.org/dc/elements/1.1/\" rdf:about=\"\">\n";
    os << "<dc:creator>\n";
    os << "<rdf:Seq>\n";
    os << "<rdf:li>Author</rdf:li>\n";
    os << "</rdf:Seq>\n";
    os << "</dc:creator>\n";
    os << "<dc:description>\n";
    os << "<rdf:Alt>\n";
    os << "<rdf:li xml:lang=\"x-default\">Subject</rdf:li>\n";
    os << "</rdf:Alt>\n";
    os << "</dc:description>\n";
    os << "<dc:title>\n";
    os << "<rdf:Alt>\n";
    os << "<rdf:li xml:lang=\"x-default\">Title</rdf:li>\n";
    os << "</rdf:Alt>\n";
    os << "</dc:title>\n";
    os << "</rdf:Description>\n";
    os << "<rdf:Description xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" rdf:about=\"\" xmp:CreatorTool=\"Creator\" xmp:CreateDate=\"2020-08-07T12:46:17Z\" xmp:ModifyDate=\"2020-08-07T12:46:17Z\"/>\n";
    os << "<rdf:Description xmlns:pdf=\"http://ns.adobe.com/pdf/1.3/\" rdf:about=\"\" pdf:Producer=\"Producer\" pdf:Keywords=\"Keywords\"/>\n";
    os << "<rdf:Description xmlns:pdfaid=\"http://www.aiim.org/pdfa/ns/id/\" rdf:about=\"\" pdfaid:part=\"1\" pdfaid:conformance=\"A\"/>\n";
    os << "</rdf:RDF>\n";
    os << "</x:xmpmeta>";

    std::string stream = os.str();



    _xmp = addXrefEntry(-1, true);
    xprintf(            // The page contents stream
        "<<\n/Type /Metadata /Subtype /XML /Length %d >>\n"     
        "stream"                 // Beginning of stream
        "\n%s\n"
        "endstream\n"              // End of stream
        "endobj\n",
        stream.size(), stream.c_str());

}

int PdfWriter::writeOutputIntents() {

    // should determine color components of ICC 
    int nColorComp = 3;

    int iccSize = 0;
    unsigned char * iccData = adobeicc(iccSize);

    int iccProfile = addXrefEntry(-1, true);
    xprintf(            // The page contents stream
        "<<\n/N %d /Length %d\n>>\n"
        "stream"                 // Beginning of stream
        "\n", nColorComp, iccSize);

    writeRawData(iccData, iccSize);

    xprintf("\nendstream\n"              // End of stream
        "endobj\n");


    _outputIntents = addXrefEntry(-1,true);

    xprintf("<< /N %d /Type /OutputIntent\n", nColorComp);
    xprintf("/S/GTS_PDFA1\n");
    xprintf("/DestOutputProfile %d 0 R\n", iccProfile);
    xprintf("/OutputCondition (AdobeRGB1998)\n");
    xprintf("/OutputConditionIdentifier (AdobeRGB1998)\n");
    xprintf("/Info (AdobeRGB1998)\n");

    xprintf(">>\n"
        "endobj\n");

    return _outputIntents;
}
